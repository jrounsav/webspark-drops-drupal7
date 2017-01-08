<?php
require_once('rfi_submission.inc');

class online_submission extends rfi_submission 
{
  
  protected $endpoint = "https://webforms.asu.edu/asu_edplus_middleware/submit/";

  protected function getEndpoint()
  {
    return $this->endpoint;
  }

  function __construct($data)
  {
    parent::__construct($data);
  }

  protected function check_managed_online()
  {
    $getProgramsList = asu_rfi_parse_programs();
    foreach($getProgramsList as $onlineItem){
      if($onlineItem->plancode == $this->getProgram() && $onlineItem->crmdestination !== "asu"){
        return true;
      }
    }
    return false;
  }
  
  protected function post_submission()
  {
    $managedOnline = $this->check_managed_online();

    if ($managedOnline == true) {
      $fields = $this->build_payload();
      $body = json_encode($fields);
      $mw_auth_key = variable_get('asu_rfi_form_auth_key');
      $fieldHash = sha1($mw_auth_key . '+' . $body);
      $url = $this->getEndpoint() . $fieldHash;
    } else {
      watchdog('asu_rfi', 'Node: ' . $this->getNodeId() . '- The program you are attempting to post is not available online or not in the list. Please verify the list of online degrees is imported.');
      return 0;
    }

    try {
      $ch = curl_init();
      $options = array(
        CURLOPT_URL => $url,
        CURLOPT_POST => 1,
        CURLOPT_POSTFIELDS => $body,
        CURLOPT_RETURNTRANSFER => 1,
        CURLOPT_HTTPHEADER => array('Content-Type: text/json'),
        CURLOPT_SSLVERSION => 1,
        CURLOPT_SSL_VERIFYPEER => FALSE,
      );

      curl_setopt_array($ch, $options);

      $response = curl_exec($ch);
      $curl_code = curl_getinfo($ch);

      //If response from middleware is not a success message, save the error message submission node and send message onto the confirmation page screen
      // if($response == "Access Denied - hashed value does not match any registered site"){
      if (($curl_code['http_code'] == 403 || $curl_code['http_code'] == 500)) {
        $this->report_error($response);
      }
      curl_close($ch);

      if (FALSE === $response) {
        throw new Exception(curl_error($ch), curl_errno($ch));
      }
    } catch (Exception $e) {
      trigger_error(sprintf(
        'Curl failed with error #%d: %s',
        $e->getCode(), $e->getMessage()),
        E_USER_ERROR);
    }
    return 1;
  }
  
  /**
   * Builds the payload for a post to EdPlus
   * @return array|int
   */
  protected function build_payload()
  {

    $program = $this->getProgram();
    $college_code = $this->get_college_code($program);

    $student_level = $this->getStudentType();

    if($student_level == "Graduate")
    {
      $student_level = 'G';
    } else if ($student_level == "Undergraduate")
    {
      $student_level = "UG";
    } else
    {
      watchdog('asu_rfi', 'Unable to parse out the Student Level for a post to ASU Online');
      return 0;
    }

    $fieldsUpdated = array(
      'first_name' => $this->getFirstName(),
      'last_name' => $this->getLastName(),
      'program' => $college_code . '-' . $program,
      'email' => $this->getEmail(),
      'phone' => $this->getPhone(),
      'prod_test_flag' => $this->getProdTestFlag(),
      'base_url' => $this->getBaseURL(),
      'remote_nid' => $this->getNodeId(),
      'ldetect_v' => $this->getIpAddress(),
      'campaign' => $this->getDomain() . '-' . $this->getTypeOfForm() . '-' . $student_level,
      'sourceid' => $this->getNodeId(),
    );

    return $fieldsUpdated;
  }
}