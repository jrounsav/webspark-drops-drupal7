<?php
require_once('rfi_submission.inc');

class undergraduate_submission extends rfi_submission{

  protected $endpoint = "https://webforms.asu.edu/asu_rfi_dedupe/submit_rfi/";

  protected function getEndpoint()
  {
    return $this->endpoint;
  }
  
  function __construct($data)
  {
    parent::__construct($data);
  }

  protected function post_submission()
  {
    $fields = $this->build_payload();
    $body = json_encode($fields);
    $mw_auth_key = variable_get('asu_rfi_form_auth_key');
    $hashkey = sha1($mw_auth_key . '+' . $this->getNodeId());
    $url = $this->getEndpoint() . $hashkey . '/' . $this->getNodeId();

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

  }
  /**
   * Builds the payload for a post the the undergraduate middleware
   * @return array
   */
  protected function build_payload()
  {
    $fields = array(
      'source_id' => $this->getSourceId(),
      'base_url' => $this->getDomain(),
      'prod_test_flag' => $this->getProdTestFlag(),
      'first_name' => $this->getFirstName(),
      'last_name' => $this->getLastName(),
      'prospect_level' => $this->getProspectLevel(),
      'student_type' => $this->getStudentType(),
      'program' => $this->getProgram(),
      'starting_semester' => $this->getStartingSemester(),
      'email' => $this->getEmail(),
      'zip' => $this->getZip(),
      'phone' => $this->getPhone(),
      'birth_date' => $this->getBirthDate(),
      'country' => $this->getCountry(),
      'sms' => $this->getSms(),
      'comments' => $this->getComments(),
      'street' => $this->getStreet(),
      'city' => $this->getCity(),
      'state' => $this->getState(),
      'intnl_student' => $this->getIntnlStudent(),
      'veteran_status' => $this->getVeteranStatus(),
      'campus' => $this->getCampus(),
    );
    return $fields;
  }
}