<?php

/**
 * Class submissionHandler
 * Manages the passing of RFI Submissions to middlewares
 * TODO - Hook submissionHandler into the core module.
 */
class submissionHandler {

  private $submission; // RFISubmission Object
  /**
   * Map Graduate Middleware => RFISubmission
   * @var array
   */
  private $GraduateMap = array(
    'first_name' => "FirstName",
    'last_name' => "LastName",
    'program' => "Program",
    'email' => "Email",
    'phone' => "Phone",
    'prod_test_flag' => "ProdTestFlag",
    'base_url' => "BaseURL",
    'remote_nid' => "NodeID",
    'ldetect_v' => "IPAddress",
    'campaign' => "SourceID",
    'sourceid' => "NodeID",
  );

  //TODO - Add Undergraduate field map

  // TODO - When building POST, loop through the field maps checking with "isset" to build out body.
  // TODO - Fail the process if !isset

  /**
   * submissionHandler constructor.
   * @param $submission_object
   * @throws Exception
   */
  public function __construct($submission_object){
    if (!$this->setSubmission($submission_object)){
      throw new Exception("The Submission Handler constructor can only accept an RFISubmission");
    }
  }

  /**
   * Set an RFISubmission to be handled by the submissionHandler.
   * Throw an exception
   * @param $data
   * @return bool
   * @throws Exception
   */
  public function setSubmission($data){
    if(is_a($data, "RFISubmission")){
      $this->submission = $data;
      return true;
    } else {
      return false;
    }
  }

  /**
   * Using the configurations, send the RFI to it's destination
   * TODO - Map RFISubmission fields to correct POST fields.
   * TODO - POST data.
   */
  public function submit(){
    if(isset($this->submission)){

    }
  }
  /**
   * Set the request headers & destination url
   */
  public function setPOST($url, $body){

    /*$options = array(
      CURLOPT_URL => $url,
      CURLOPT_POST => 1,
      CURLOPT_POSTFIELDS => $body,
      CURLOPT_RETURNTRANSFER => 1,
      CURLOPT_HTTPHEADER => array('Content-Type: text/json'),
      CURLOPT_SSLVERSION => 1,
      CURLOPT_SSL_VERIFYPEER => FALSE,
    );



    curl_setopt($curl, CURLOPT_HEADER, FALSE); //Set this to false to remove informational headers*/
  }

  public function setURL($prospectLevel){

    switch ($prospectLevel) {
      case "G":
        /*$url = "https://webforms.asu.edu/asu_edplus_middleware/submit/$fieldHash";*/
        break;
      case "UG":
        /*$url = "https://webforms.asu.edu/asu_rfi_dedupe/submit_rfi/$hashkey/$nodeid";*/
        break;
      default:

        break;
    }

  }

  public function generateGradHash($authKey, $data){
    $submission_data = json_encode($data);

    $hash = sha1($authKey . '+' . $submission_data);

    return $hash;
  }

  public function generateUndergradHash($authKey, $nodeid){
    $hash = sha1($authKey . '+' . $nodeid);

    return $hash;
  }

  public function buildUndergradBody(){
    /*$fields = array(
      'source_id' => $source,
      'base_url' => $domain,
      'prod_test_flag' => $form_mode,
      'first_name' => $fname,
      'last_name' => $lname,
      'prospect_level' => 'Undergraduate',
      'student_type' => $data['student_type'],
      'program' => $prog_code,
      'starting_semester' => $data['start_date'],
      'email' => $email,
      'zip' => $data['zipcode'],
      'phone' => $phone,
      'birth_date' => $dob,
      'country' => $country,
      'sms' => $mobile,
      'comments' => $comment,
      'prod_test_flag' => $form_mode,
      'street' => $address,
      'city' => $city,
      'state' => $state,
      'intnl_student' => $data['citizenship'],
      'veteran_status' => $military,
      'campus' => $campus,
      'location' => $location,
    );*/
  }

  public function buildGradBody(){


  }
  /**
   * Configure fields to be used by POST Functions
   */





}
