<?php

/**
 * Interface submission
 */
class RFISubmission {

  /**
   * Required across all types of submissions
   * @var array
   */
  public $require = array("Program", "FirstName", "LastName", "Phone", "Email", "SourceID");

  // TODO - decide what should happen in the constructor, if anything
  public function __construct()
  {

  }

  /**
   * Validate whether all of the submitted data points meet requirements
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
   * @param $data
   * @return array
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
   */
  public function setProdTestFlag(){}
  public function getProdTestFlag(){}
  public function validateProdTestFlag($data){

    if (filter_var($data, FILTER_VALIDATE_BOOLEAN)) {
      return true;
    }

    return false;
  }

  /**
   * College Code
   */
  public function setCollege(){}
  public function getCollege(){}
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
   * verify program exists locally
   */
  public function setProgram(){}
  public function getProgram(){}
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
   * Grad/Undergrad
   */
  public function setProspectLevel(){}
  public function getProspectLevel(){}
  public function validateProspectLevel($data){

    if($data == "G" || $data == "UG"){
      return true;
    }

    return false;
  }

  /**
   * Location - dependent on program & prospect level
   */
  public function setCampus(){}
  public function getCampus(){}
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
   * Needs to be a domain with  "-RFIAPI" appended
   * TODO - Need to find a way to append UG/G
   */
  public function setSourceID(){}
  public function getSourceID(){}
  public function validateSourceID($data){

    if(!preg_match('/[\'^£$%&*()}{@#~?><>,|=_+¬]/', $data) && strpos($data, "-RFIAPI")){
      return true;
    }

    return false;
  }

  /**
   * Base URL - String
   * TODO - Exclude and populate on backend - Maybe allow override?
   */
  public function setBaseURL(){}
  public function getBaseURL(){}
  public function validateBaseURL(){

    return false;
  }

  /**
   * String
   */
  public function setFirstName(){}
  public function getFirstName(){}
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
   * String
   */
  public function setLastName(){}
  public function getLastName(){}
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
   * Verify form of email
   */
  public function setEmail(){}
  public function getEmail(){}
  public function validateEmail($data){

    if (filter_var($data, FILTER_VALIDATE_EMAIL)) {
      if(strlen($data) < 128){
        return true;
      }
    }

    return false;
  }

  /**
   * Verify phone number. No letters
   */
  public function setPhone(){}
  public function getPhone(){}
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
   * Returning, Transfer, Current, Freshman
   */
  public function setStudentType(){}
  public function getStudentType(){}
  public function validateStudentType($data){

    if($data == "First Time Freshman" || $data == "Transfer" || $data == "Readmission"){
      return TRUE;
    }

    return false;
  }

  /** - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
   * TODO - Figure out best way to get better Semester Codes & Location Information.
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
   * TODO END - Figure out best way to get better Semester Codes & Location Information.
   * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /**
   * Birth date
   */
  public function setBirthdate(){}
  public function getBirthdate(){}
  public function validateBirthdate($data){

    if(preg_match('/^(0[1-9]|1[0-2])\/(0[1-9]|1\d|2\d|3[01])\/(19|20)\d{2}$/', $data)){
      return TRUE;
    }

    return false;
  }

  /**
   * Accept SMS Notifications
   */
  public function setSMS(){}
  public function getSMS(){}
  public function validateSMS($data){

    if (filter_var($data, FILTER_VALIDATE_BOOLEAN)) {
      return true;
    }
    return false;
  }

  /**
   * Comment string
   */
  public function setComments(){}
  public function getComments(){}
  public function validateComments($data){

    if(!empty($data)){
      return TRUE;
    }
    return false;
  }

  /**
   * International Student? - Boolean
   */
  public function setInternationalStudent(){}
  public function getInternationalStudent(){}
  public function validateInternationalStudent($data){

    if (filter_var($data, FILTER_VALIDATE_BOOLEAN)) {
      return true;
    }
    return false;
  }

  /**
   * Military Status
   */
  public function setVeteranStatus(){}
  public function getVeteranStatus(){}
  public function validateVeteranStatus($data){

    if (filter_var($data, FILTER_VALIDATE_BOOLEAN)) {
      return true;
    }
    return false;
  }

  /**
   * ---- ---- ---- ----  ---- ---- ---- ----
   */


}
