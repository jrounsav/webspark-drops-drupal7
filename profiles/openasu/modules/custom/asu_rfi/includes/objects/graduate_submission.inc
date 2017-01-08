<?php
require_once('rfi_submission.inc');

class graduate_submission extends rfi_submission 
{
  
  protected $endpoint = 'https://requestinfo.asu.edu/prospect_form_post';
  
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
    $submission_data = $this->build_payload();
    $submission_data = json_encode($submission_data);
    try {
      //url to post data to salesforce
      $curl = curl_init($this->getEndpoint());
      curl_setopt($curl, CURLOPT_RETURNTRANSFER, TRUE); //If you don't want to use any of the return information, set to false
      curl_setopt($curl, CURLOPT_HEADER, FALSE); //Set this to false to remove informational headers
      curl_setopt($curl, CURLOPT_CUSTOMREQUEST, 'POST');
      curl_setopt($curl, CURLOPT_POSTFIELDS, $submission_data); //data mapping
      curl_setopt($curl, CURLOPT_SSLVERSION, 1); //This will set the security protocol to TLSv1
      curl_setopt($curl, CURLOPT_SSL_VERIFYPEER, false);
      $response = curl_exec($curl);

      $info = curl_getinfo($curl); //Used by your drupal messages below

      curl_close($curl);

      if (strpos($response, "-1 ") == 0) {
        $message = explode(':', $response);
      } else {
        $message = "broke";
      }

      //Get the response from Salesforce. If the submission is not successfull, update the previous submission status to Incomplete
      if (($info['http_code'] == 400) || ($info['http_code'] == 500)) {
        //update error message
        $this->report_error($message[1]);
      }

      if (($info['http_code'] == 200)) {
        $this->setErrorResponse('');
        //if succesfully to post to SF, the set "last_failure_SF_posting" variable to current time
        variable_set('last_success_SF_posting', time());
      }

      //if failed to post to SF, the set "last_failure_SF_posting" variable to current time
      if (($info['http_code'] != 200)) {
        variable_set('last_failure_SF_posting', time());
      }
    } catch (Exception $e) {
      
    }
  }
  /**
   * Builds the payload for a post the the graduate middleware
   * @return array
   */
  protected function build_payload()
  {
    $fields = array(
      'source' => $this->getSourceId(),
      'firstName' => $this->getFirstName(),
      'lastName' => $this->getLastName(),
      'emailAddress' => $this->getEmail(),
      'phoneNumber' => $this->getPhone(),
      'projectedEnrollment' => $this->getProspectLevel(),
      'addressLine1' => $this->getStreet(),
      'city' => $this->getCity(),
      'state' => $this->getState(),
      'countryOfResidence' => $this->getCountry(),
      'birthdate' => $this->getBirthDate(),
      'militaryStatus' => $this->getVeteranStatus(),
      'questions' => $this->getComments(),
      'poiCode' => $this->getProgram(),
      'zip' => $this->getZip(),
      'iStudent' => $this->getIntnlStudent(),
      'smsNumber' => $this->getSms(),
      'campusSelection' => $this->getCampus(),
    );

    return $fields;
  }
}