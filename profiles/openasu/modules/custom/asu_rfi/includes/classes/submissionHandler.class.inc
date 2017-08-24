<?php

/**
 * Class submissionHandler
 * Manages the passing of RFI Submissions to middlewares
 * TODO - submit() set to stop and return false before submitting. Handle "False" flags.
 * TODO - Add IPAddress to "required" once the IP bug is fixed.
 */
class submissionHandler {

  /**
   * @var RFISubmission Object
   */
  public $submission;
  /**
   * @var array of ASUOnline managed degrees
   */
  public $onlineCatalog;

  /**
   * @var boolean True if the submission is in test mode.
   */
  public $testMode;

  /**
   * @var String authorization key for the middleware.
   */
  public $authKey;

  /** ================================================
   * Middleware to RFISubmission Field Mappings.
   * Field Requirements between differ, so we need to map the standard RFISubmission fields to the necessary
   * middleware's field names before posting.
   *  ================================================ */
  /**
   * Map Graduate Middleware => RFISubmission
   * @var array
   */
  public $GraduateMap = array(
    "requirements" => array("FirstName", "LastName", "Program", "Email", "Phone", "BaseURL", "TypeOfForm", "ProspectLevel"),
    "map" => array(
      'first_name' => "FirstName",
      'last_name' => "LastName",
      'program' => "Program",
      'email' => "Email",
      'phone' => "Phone",
      'prod_test_flag' => "ProdTestFlag",
      'base_url' => "BaseURL",
      'remote_nid' => "NodeID",
      'ldetect_v' => "IPAddress",
      'campaign' => "BaseURL-TypeOfForm-ProspectLevel",
      'sourceid' => "NodeID",
    )
  );

  /**
   * Map Undergraduate Middleware => RFISubmission
   * @var array
   */
  public $UndergraduateMap = array(
    "requirements" => array("BaseURL", "TypeOfForm", "FirstName", "LastName",
      "ProspectLevel", "Program", "StudentType", "StartingSemester", "Email", "Phone", "Birthdate"),
    "map" => array(
      'source_id' => "BaseURL-TypeOfForm-ProspectLevel",
      'base_url' => "BaseURL",
      'prod_test_flag' => "ProdTestFlag",
      'first_name' => "FirstName",
      'last_name' => "LastName",
      'prospect_level' => 'ProspectLevelFull',
      'student_type' => "StudentType",
      'program' => "Program",
      'starting_semester' => "StartingSemester",
      'email' => "Email",
      'zip' => "Zipcode",
      'phone' => "Phone",
      'birth_date' => "Birthdate",
      'country' => "Country",
      'sms' => "SMS",
      'comments' => "Comments",
      'street' => "Street",
      'city' => "City",
      'state' => "State",
      'intnl_student' => "InternationalStudent",
      'veteran_status' => "VeteranStatus",
      'campus' => "Campus",
    )
  );

  /**
   * Map ASUOnline Middleware => RFISubmission
   * @var array
   */
  public $OnlineMap = array(
    "requirements" => array("FirstName", "LastName", "Program", "College", "Email", "Phone", "BaseURL", "TypeOfForm", "ProspectLevel"),
//    "requirements" => array("FirstName", "LastName", "Program", "College", "Email", "Phone", "BaseURL", "TypeOfForm", "ProspectLevel", "IPAddress"),
    "map" => array(
      'first_name' => "FirstName",
      'last_name' => "LastName",
      'program' => "College-Program",
      'email' => "Email",
      'phone' => "Phone",
      'prod_test_flag' => "ProdTestFlag",
      'base_url' => "BaseURL",
      'remote_nid' => "NodeID",
      'ldetect_v' => "IPAddress",
      'campaign' => "BaseURL-TypeOfForm-ProspectLevel", //
      'sourceid' => "NodeID", // TODO - Implement some sort of UUID setter getter
    )
  );

  /** ================================================
   * RFI Submission Handler Initialization Functions
   *  ================================================ */

  /**
   * submissionHandler constructor.
   * @param $submission_object
   * @param $onlineCatalog
   * @param bool $testMode
   * @param null $authKey
   * @throws Exception
   */
  public function __construct($submission_object, $onlineCatalog, $authKey = ""){
    if (!$this->setSubmission($submission_object)){
      throw new Exception("The Submission Handler constructor failed to validate your submission");
    }
    if (!$this->setOnlineCatalog($onlineCatalog)){
      throw new Exception("The Submission Handler failed to set the Online Catalog");
    }
    if(!$this->setAuthKey($authKey)){
      throw new Exception("The supplied authorization key is not acceptable for the submissionHandler.");
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
    if($this->validateSubmission($data)){
      $this->submission = $data;
      return true;
    } else {
      return false;
    }
  }
  public function validateSubmission($data = null){
    if(!isset($data)){
      $data = $this->submission;
    }
//    if(is_a($data, "RFISubmission") && $this->validateSubmissionBody($data)){
    if(is_a($data, "RFISubmission")){
      return true;
    } else {
      return false;
    }
  }

  /**
   * Validate the submission body based on the required path
   * @param $data
   * @return bool
   */
  public function validateSubmissionBody($data){

    $route = $this->determineRoute($data);
    $map = $this->determineMap($route);

    $body = [1,2];
    $mapping = $map["map"];

    foreach($mapping as $field => $functionBase){
      $bases = explode("-", $functionBase);
      $full = "";

      for($i = 0; $i < sizeof($bases); $i++) {
        $function = "get" . $bases[$i];

        if(!$data->$function() && in_array($bases[$i], $map["requirements"])){
          return false;
        }

        $response = $data->$function();
        $full .= $response;
        if($i < sizeof($bases) - 1 && sizeof($bases) > 1){
          $full .= "-";
        }
      }

      $body[$field] = $full;
    }

    if(count($body) > 0){
      return true;
    }
    return false;
  }

  /**
   * Set handler's list of managed online programs. Necessary to determine submission data route.
   * can call asu_rfi_parse_programs() and pass data in constructor.
   * MUST be set before attempting to submit Online request.
   * @param $data
   * @return bool
   */
  public function setOnlineCatalog($data){
    if($this->validateOnlineCatalog($data)){
      $this->onlineCatalog = $data;
      return true;
    } else {
      return false;
    }
  }
  public function validateOnlineCatalog($data){
    if(is_array($data)){
      return true;
    } else {
      return false;
    }
  }

  /**
   * Middleware Authorization Key
   * This is necessary for posts to the various middleware endpoints.
   * @param $data
   * @return bool
   */
  public function setAuthKey($data){
    if($this->validateAuthKey($data)){
      $this->authKey = $data;
      return true;
    } else {
      return false;
    }
  }
  public function validateAuthKey($data){
    if(is_string($data)){
      return true;
    } else {
      return false;
    }
  }

  /** ================================================
   * RFI Submission Handler Submit Functions
   * Running "submit" after the object is initialized with a submission object
   * and the Online Managed catalog should build out the submission body and
   * send it to the correct endpoint
   *  ================================================ */

  /**
   * Using the submission data,prepare and submit the request.
   * @return array|bool
   */
  public function submit(){
    if(isset($this->submission)){
      $route = $this->determineRoute();
      $map = $this->determineMap($route);
      $body = $this->setPOSTBody($map);
      $url = $this->generateURLForRoute($route, $body);
      $options = $this->setPOSTOptions($url, $body);

      $response = $this->curl($options);

      return $response;
    }
  }

  /**
   * Run cURL against the middleware endpoint, with the submission data, and return the response and code
   * @param $options
   * @return array
   */
  public function curl($options){

    $values = [];

    $ch = curl_init();

    curl_setopt_array($ch, $options);

    $response = curl_exec($ch);
    $curl_code = curl_getinfo($ch);

    curl_close($ch);

    array_push($values, $response, $curl_code);

    return $values;

  }

  /**
   * Determine functions choose the route, as well as choose the map. Maybe it would be better to set some local variable
   * instead of returning some 1-off. This is a task for another day I guess.
   * @return bool|string
   */
  public function determineRoute($submission = null){
    if(!isset($submission)){
      $submission = $this->submission;
    }

    $campus = $submission->getCampus();
    $program = $submission->getProgram();

    $managed = $this->checkOnlineManaged($program);
    if($campus == "ONLNE" && $managed){
      $submission->setCollege($managed);
      return "O";
    } else {
      return $submission->getProspectLevel();
    }
  }
  public function determineMap($route){
    switch ($route) {
      case "G":
        $map = $this->GraduateMap;
        break;
      case "UG":
        $map = $this->UndergraduateMap;
        break;
      case "O":
        $map = $this->OnlineMap;
        break;
      default:
        return false;
        break;
    }

    return $map;
  }

  /**
   * Check whether a program is in the provided list of ASUOnline managed programs.
   * @param $program
   * @return bool|String If it's managed return the managing college, else return false.
   */
  public function checkOnlineManaged($program)
  {
    if (isset($this->onlineCatalog)) {
      foreach ($this->onlineCatalog as $item) {
        if ($program == $item->plancode && $item->crmdestination !== "asu") {
          return $item->progcode ;
        }
      }
    }
    return false;
  }

  /**
   * Build out the POST options for the cURL to be utilized
   * @param $url
   * @param $body
   * @return array
   */
  public function setPOSTOptions($url, $body){

    $options = array(
      CURLOPT_URL => $url,
      CURLOPT_POST => 1,
      CURLOPT_POSTFIELDS => $body,
      CURLOPT_RETURNTRANSFER => 1,
      CURLOPT_HTTPHEADER => array('Content-Type: text/json'),
      CURLOPT_SSLVERSION => 1,
      CURLOPT_SSL_VERIFYPEER => FALSE,
      CURLOPT_HEADER => FALSE
    );

    return $options;

  }

  /**
   * Builds the post body by generating the function names in the RFISubmission class.
   * @param $map
   * @return array|bool
   */
  public function setPOSTBody($map){
    $body = [];
    $mapping = $map["map"];

    foreach($mapping as $field => $functionBase){
      $bases = explode("-", $functionBase);
      $full = "";

      for($i = 0; $i < sizeof($bases); $i++) {
        $function = "get" . $bases[$i];

        if(!$this->submission->$function() && in_array($bases[$i], $map["requirements"])){
          dpm($bases[$i]);
          return false;
        }

        $response = $this->submission->$function();
        $full .= $response;
        if($i < sizeof($bases) - 1 && sizeof($bases) > 1){
          $full .= "-";
        }
      }

      $body[$field] = $full;
    }

    return $body;
  }


  /**
   * Build the destination URL for the submission
   * @param $route
   * @param $data
   * @return bool|string
   *
   */
  public function generateURLForRoute($route, $data){
    $url = false;
    switch ($route) {
      case "G":
        if($this->submission->getProdTestFlag() == "prod"){
          $url = 'https://requestinfo.asu.edu/prospect_form_post';
        } else {
          $url = 'https://requestinfo-qa.asu.edu/prospect_form_post';
        }
        break;
      case "UG":
        if($this->submission->getNodeID()){
          if($this->submission->getProdTestFlag() == "prod"){
            $environment = "https://webforms.asu.edu/asu_rfi_dedupe/submit_rfi";
          } else {
            $environment = "https://webforms-qa.asu.edu/asu_rfi_dedupe/submit_rfi";
          }
          $nodeid = $this->submission->getNodeID();
          $hash = $this->generateUndergradHash($this->submission->getNodeID());
          $url = "$environment/$hash/$nodeid";
        }
        break;
      case "O":
        if(!empty($data)) {
          if($this->submission->getProdTestFlag() == "prod"){
            $environment = "https://webforms.asu.edu/asu_edplus_middleware/submit";
          } else {
            $environment = "https://webforms-qa.asu.edu/asu_edplus_middleware/submit";
          }
          $hash = $this->generateOnlineHash($data);
          $url = "$environment/$hash";
        }
        break;
      default:
        $url = false;
        break;
    }
    return $url;

  }


  /** ================================================
   * Middleware Hash Generators
   * The middleware endpoints utilize different hashing processes.
   *  ================================================ */

  public function generateUndergradHash($nodeid){
    $authKey = $this->authKey;
    $hash = sha1($authKey . '+' . $nodeid);

    return $hash;
  }

  public function generateOnlineHash($data){
    $authKey = $this->authKey;
    $submission_data = json_encode($data);

    $hash = sha1($authKey . '+' . $submission_data);

    return $hash;
  }

  /**
   * Currently Non-existent. Placeholder for a Grad hash
   * @param $data
   */
  public function generateGradHash($data){}

}
