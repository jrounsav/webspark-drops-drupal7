<?php

/**
 * Interface submission
 */
class RFISubmission {

  private $prodTestFlag;
  private $nodeID;
  private $IPaddress;
  private $college;
  private $program;
  private $prospectLevel;
  private $campus;
  private $sourceID;
  private $baseURL;
  private $firstName;
  private $lastName;
  private $email;
  private $phone;
  private $studentType;
  private $birthdate;
  private $SMS;
  private $comments;
  private $internationalStudent;
  private $veteranStatus;

  /**
   * Required across all types of submissions
   * @var array
   */
  public $require = array("Program", "FirstName", "LastName", "Phone", "Email", "SourceID", "College");

  /**
   * Validate whether all of the submitted data points meet requirements
   * @param $data
   * @return array|mixed
   */
  public function validate($data){

    $response = $this->validateInput($data);

    if($response['status'] == 1){
      $response = $this->validateRequirements($response);
    }

    return $response;

  }

  /**
   * Validate the input. Is it sanitary? Does it meet RegEx standards?
   * Is it the correct type of data?
   * @param $data
   * @return array
   */
  public function validateInput($data){

    $valid = array();

    $response = array(
      "status" => 1,
      "response" => "Bad Data. Submission failed on POST key: "
    );

    foreach($data as $k => $v){
      $strName = "validate" . $k;

      if(method_exists($this, $strName)){

        if($this->$strName($v)){
          $valid[$k] = $v;
        } else {
          $response['status'] = 0;
          $response['response'] = $response['response'] . $k . ', ';
        }

      } else {
        $response['status'] = 0;
        $response['response'] = $response['response'] . $k . ', ';
      }
    }

    if($response['status'] == 1){
      $response['response'] = "Fields are correct";
    }

    $response['data'] = $valid;

    return $response;

  }

  /**
   * Verify that the required fields are included.
   * @param $response
   * @return mixed
   *
   * TODO - expand this to return all missing fields
   */
  public function validateRequirements($response){

    foreach($this->require as $v){
      if(!array_key_exists($v, $response['data'])){
        // If missing required values, break the loop and send the failure
        $response['status'] = 0;
        $response['response'] = "Your submission is missing a required field: " . $v;
        return $response;
      }
    }

    return $response;

  }

  /**
   * ---- ---- ---- VALIDATE/SET/GET Single Fields --- --- --- --- ---
   */

  /**
   * Testing? Boolean, On/Off
   * @param $data
   * @return bool
   */
  public function setProdTestFlag($data){
    if($this->validateProdTestFlag($data)){
      $this->prodTestFlag = $data;
      return true;
    } else {
      return false;
    }
  }
  public function getProdTestFlag(){
    if(isset($this->prodTestFlag)){
      return $this->prodTestFlag;
    } else {
      return false;
    }
  }
  public function validateProdTestFlag($data){

    if ($data == "TRUE" || $data == "FALSE") {
      return true;
    }

    return false;
  }

  /**
   * Site node ID
   * @param $data
   * @return bool
   */
  public function setNodeID($data){
    if($this->validateNodeID($data)){
      $this->nodeID = $data;
      return true;
    } else {
      return false;
    }
  }
  public function getNodeID(){
    if(isset($this->nodeID)){
      return $this->nodeID;
    } else {
      return false;
    }
  }
  public function validateNodeID($data){

    if (is_numeric($data)) {
      return true;
    }

    return false;
  }

  /**
   * Submitter's IP Address
   * @param $data
   * @return bool
   */
  public function setIPAddress($data){
    if($this->validateIPAddress($data)){
      $this->IPaddress = $data;
      return true;
    } else {
      return false;
    }
  }
  public function getIPAddress(){
    if(isset($this->IPaddress)){
      return $this->IPaddress;
    } else {
      return false;
    }
  }
  public function validateIPAddress($data){

    if (is_numeric($data)) {
      return true;
    }

    return false;
  }

  /**
   * College Code
   * @param $data
   * @return bool
   */
  public function setCollege($data){
    if($this->validateCollege($data)){
      $this->college = $data;
      return true;
    } else {
      return false;
    }
  }
  public function getCollege(){
    if(isset($this->college)){
      return $this->college;
    } else {
      return false;
    }
  }
  public function validateCollege($data){

    if (!preg_match('/[\'^£$%&*()}{@#~?><>,|=_+¬-]/', $data))
    {
      if(strlen($data) < 6){
        return true;
      }
    }

    return false;
  }

  /**
   * ASU managed program code
   * @param $data
   * @return bool
   */
  public function setProgram($data){
    if($this->validateProgram($data)){
      $this->program = $data;
      return true;
    } else {
      return false;
    }
  }
  public function getProgram(){
    if(isset($this->program)){
      return $this->program;
    } else {
      return false;
    }
  }
  public function validateProgram($data){

    // So special characters allowed
    if (!preg_match('/[\'^£$%&*()}{@#~?><>,|=_+¬-]/', $data))
    {
      // 16 is an arbitrary number. I don't know the real limit
      if(strlen($data) < 16){
        return true;
      }
    }
    return false;
  }

  /**
   * Student Prospect Level. Graduate or Undergraduate
   * @param $data
   * @return bool
   */
  public function setProspectLevel($data){
    if($this->validateProspectLevel($data)){
      $this->prospectLevel = $data;
      return true;
    } else {
      return false;
    }
  }

  public function getProspectLevel(){
    if(isset($this->prospectLevel)){
      return $this->prospectLevel;
    } else {
      return false;
    }
  }

  public function getProspectLevelFull(){
    if(isset($this->prospectLevel)){
      switch ($this->prospectLevel) {
        case "G":
          return "Graduate";
          break;
        case "UG":
          return "Undergraduate";
          break;
        default:
          return false;
          break;
      }
    } else {
      return false;
    }
  }

  public function validateProspectLevel($data){

    if($data == "G" || $data == "UG"){
      return true;
    }

    return false;
  }

  /**
   * Campus code
   * @param $data
   * @return bool
   */
  public function setCampus($data){
    if($this->validateCampus($data)){
      $this->campus = $data;
      return true;
    } else {
      return false;
    }
  }
  public function getCampus(){
    if(isset($this->campus)){
      return $this->campus;
    } else {
      return false;
    }
  }
  public function validateCampus($data){

    if (!preg_match('/[\'^£$%&*()}{@#~?><>,|=_+¬-]|[[0-9]/', $data))
    {
      // 16 is an arbitrary number. I don't know the real limit
      if(strlen($data) < 16){
        return true;
      }
    }
    return false;
  }

  /**
   * Source - String. Domain-TypeOfForm-UG/G
   * @param $data
   * @return bool
   * TODO - Replace validation here with Domain-TypeOfForm-UG/G - TypeOfForm will represent whether its an API call
   */
  public function setSourceID($data){
    if($this->validateSourceID($data)){
      $this->sourceID = $data;
      return true;
    } else {
      return false;
    }
  }
  public function getSourceID(){
    if(isset($this->sourceID)){
      return $this->sourceID;
    } else {
      return false;
    }
  }
  public function validateSourceID($data){

    if(!preg_match('/[\'^£$%&*()}{@#~?><>,|=_+¬]/', $data) && strpos($data, "-RFIAPI")){
      return true;
    }

    return false;
  }

  /**
   * Website's Base URL, for submission validation to the middleware
   * @param $data
   * @return bool
   */
  public function setBaseURL($data){
    if($this->validateBaseURL($data)){
      $this->baseURL = $data;
      return true;
    } else {
      return false;
    }
  }
  public function getBaseURL(){
    if(isset($this->baseURL)){
      return $this->baseURL;
    } else {
      return false;
    }
  }
  public function validateBaseURL($data){

    if ($data){
      return false;
    }
    return false;
  }

  /**
   * First name of requester
   * @param $data
   * @return bool
   */
  public function setFirstName($data){
    if($this->validateFirstName($data)){
      $this->firstName= $data;
      return true;
    } else {
      return false;
    }
  }
  public function getFirstName(){
    if(isset($this->firstName)){
      return $this->firstName;
    } else {
      return false;
    }
  }
  public function validateFirstName($data){

    if (!preg_match('/[\^£$%&*()}{@#~?><>,|=_+¬]|[[0-9]/', $data))
    {
      if(strlen($data) < 32){
        return true;
      }
    }
    return false;
  }

  /**
   * Requester Last Name
   * @param $data
   * @return bool
   */
  public function setLastName($data){
    if($this->validateLastName($data)){
      $this->lastName = $data;
      return true;
    } else {
      return false;
    }
  }
  public function getLastName(){
    if(isset($this->lastName)){
      return $this->lastName;
    } else {
      return false;
    }
  }
  public function validateLastName($data){

    if (!preg_match('/[\^£$%&*()}{@#~?><>,|=_+¬]|[[0-9]/', $data))
    {
      if(strlen($data) < 32){
        return true;
      }
    }
    return false;
  }

  /**
   * Requester's Email address
   * @param $data
   * @return bool
   */
  public function setEmail($data){
    if($this->validateEmail($data)){
      $this->email = $data;
      return true;
    } else {
      return false;
    }
  }
  public function getEmail(){
    if(isset($this->email)){
      return $this->email;
    } else {
      return false;
    }
  }
  public function validateEmail($data){

    if (filter_var($data, FILTER_VALIDATE_EMAIL)) {
      if(strlen($data) < 128){
        return true;
      }
    }

    return false;
  }

  /**
   * Requester's phone number.
   * @param $data
   * @return bool
   */
  public function setPhone($data){
    if($this->validatePhone($data)){
      $this->phone = $data;
      return true;
    } else {
      return false;
    }
  }
  public function getPhone(){
    if(isset($this->phone)){
      return $this->phone;
    } else {
      return false;
    }
  }
  public function validatePhone($data){

    // Some coutries use + and x in their phone numbers. Must allow
    if (!preg_match('/[^\d+x]|x(?=[^x]*x)/', $data))
    {
      if(strlen($data) < 16){
        return true;
      }
    }
    return false;
  }

  /**
   * The type of student with respect to their Higher Ed history. Freshman, Transfer, Readmission
   * @param $data
   * @return bool
   */
  public function setStudentType($data){
    if($this->validateStudentType($data)){
      $this->studentType = $data;
      return true;
    } else {
      return false;
    }
  }
  public function getStudentType(){
    if(isset($this->studentType)){
      return $this->studentType;
    } else {
      return false;
    }
  }
  public function validateStudentType($data){

    if($data == "First Time Freshman" || $data == "Transfer" || $data == "Readmission"){
      return TRUE;
    }

    return false;
  }

  /** - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
   * The semester codes, Countries, & States need to be validated against a list of acceptable options.
   *
   * Everything in this block is contingent on a non-submission resource.
   * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /**
   * Starting Semester
   * TODO - validate form of semester code. Not accepting for now
   */
  public function setStartingSemester(){}
  public function getStartingSemester(){}
  public function validateStartingSemester(){

    return false;
  }

  /**
   * Country - Tie in with SF countries somehow
   * TODO - validate against list of countries. Not accepting for now
   */
  public function setCountry(){}
  public function getCountry(){}
  public function validateCountry(){

    return false;
  }

  /**
   * Zip Code
   * TODO - validate zip form. Not accepting for now
   */
  public function setZipcode(){}
  public function getZipcode(){}
  public function validateZipcode(){

    return false;
  }

  /**
   * Street
   * TODO - Validate string, extract improper special characters. Not accepting for now
   */
  public function setStreet(){}
  public function getStreet(){}
  public function validateStreet(){

    return false;
  }

  /**
   * City
   * TODO - Validate string, extract improper special characters. Not accepting for now
   */
  public function setCity(){}
  public function getCity(){}
  public function validateCity(){

    return false;
  }

  /**
   * State
   * TODO - Validate string, extract improper special characters. Not accepting for now
   */
  public function setState(){}
  public function getState(){}
  public function validateState(){

    return false;
  }
  /** - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
   * TODO - Confirm that the API calls work on client sites. If so, hook into DB values for data validation
   * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /**
   * Requester's date of birth
   * @param $data
   * @return bool
   */
  public function setBirthdate($data){
    if($this->validateBirthdate($data)){
      $this->birthdate = $data;
      return true;
    } else {
      return false;
    }
  }
  public function getBirthdate(){
    if(isset($this->birthdate)){
      return $this->birthdate;
    } else {
      return false;
    }
  }
  public function validateBirthdate($data){

    if(preg_match('/^(0[1-9]|1[0-2])\/(0[1-9]|1\d|2\d|3[01])\/(19|20)\d{2}$/', $data)){
      return TRUE;
    }

    return false;
  }

  /**
   * The phone number to be used if a user would like to receive info via SMS
   * @param $data
   * @return bool
   */
  public function setSMS($data){
    if($this->validateSMS($data)){
      $this->SMS = $data;
      return true;
    } else {
      return false;
    }
  }
  public function getSMS(){
    if(isset($this->SMS)){
      return $this->SMS;
    } else {
      return false;
    }
  }
  public function validateSMS($data){

    // Some coutries use + and x in their phone numbers. Must allow
    if (!preg_match('/[^\d+x]|x(?=[^x]*x)/', $data))
    {
      if(strlen($data) < 16){
        return true;
      }
    }
    return false;
  }

  /**
   * Additional comments by requester for questions & info
   * @param $data
   * @return bool
   */
  public function setComments($data){
    if($this->validateComments($data)){
      $this->comments = $data;
      return true;
    } else {
      return false;
    }
  }
  public function getComments(){
    if(isset($this->comments)){
      return $this->comments;
    } else {
      return false;
    }
  }
  public function validateComments($data){

    if(!empty($data)){
      return TRUE;
    }
    return false;
  }

  /**
   * Is the student from outside of the US
   * @param $data
   * @return bool
   */
  public function setInternationalStudent($data){
    if($this->validateInternationalStudent($data)){
      $this->internationalStudent= $data;
      return true;
    } else {
      return false;
    }
  }
  public function getInternationalStudent(){
    if(isset($this->internationalStudent)){
      return $this->internationalStudent;
    } else {
      return false;
    }
  }
  public function validateInternationalStudent($data){

    if ($data == "TRUE" || $data == "FALSE") {
      return true;
    }
    return false;
  }

  /**
   * The requester's military status.
   * @param $data
   * @return bool
   */
  public function setVeteranStatus($data){
    if($this->validateVeteranStatus($data)){
      $this->veteranStatus = $data;
      return true;
    } else {
      return false;
    }
  }
  public function getVeteranStatus(){
    if(isset($this->veteranStatus)){
      return $this->veteranStatus;
    } else {
      return false;
    }
  }
  public function validateVeteranStatus($data){

    if (in_array($data, ["Spouse/Dependent", "Veteran", "None"])) {
      return true;
    }
    return false;
  }

  /**
   * ---- ---- ---- ----  ---- ---- ---- ----
   */


}
