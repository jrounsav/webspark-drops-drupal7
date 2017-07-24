<?php

/**
 * Class SFDataManager
 *
 * Link the use of data provided by Salesforce, to the storage in Drupal.
 */
class SFDataManager {

  // Should sync ALL data points
  public function syncData(){
    $this->syncCountries();
    $this->syncStates();
    $this->syncSemesters();
  }

  /**
   * Pull Salesforce data from the middleware to be used by the RFI system.
   * @param $type String SF data type
   * @param $name String SF resource
   * @param string $middleware_host Middleware URL
   * @return mixed Result will be an array of objects, or false
   */
  public function pullData($type, $name, $middleware_host = "webforms.asu.edu/salesforce_query/api"){

    $url = "https://$middleware_host/$type/$name";

    $ch = curl_init();
    $options = array(
      CURLOPT_URL => $url,
      CURLOPT_RETURNTRANSFER => 1,
      CURLOPT_HTTPHEADER => array('Content-Type: text/json', 'Authorization: Basic 7755:e7acf2e698751f4cac11718d97df6ebf64b94372'),
    );


    curl_setopt_array($ch, $options);

    $response = curl_exec($ch);

    $result = json_decode($response);
    curl_close($ch);

    return $result;
  }

  /*
   * Handle Country data
   */
  public function checkCountryTableExists(){
    $val = db_table_exists("asu_rfi_countries");
    return $val;
  }
  public function pullCountries(){
    $type = 'fieldinfo';
    $name = 'country';

    $data = $this->pullData($type, $name);

    if(empty($data)){
      watchdog('asu_rfi', "Failed to pull Country data from the Middleware");
    }
    return $data;
  }

  public function syncCountries(){

    if($this->checkCountryTableExists()){
      $MWData = $this->pullCountries();
      $localData = $this->getCountries();
      if(!$MWData){
        watchdog('asu_rfi', "Failed to sync countries");
      } else {
        $clean = true;
        foreach($MWData as $item){
          if(isset($localData[$item->value])){
            if($localData[$item->value]->countryDesc !== $item->label){
              $clean = false;
            }
          } else {
            $clean = false;
          }
        }
        if(!$clean){
          db_truncate('asu_rfi_countries')->execute();
          foreach($MWData as $country){
            db_query("INSERT INTO {asu_rfi_countries} (countryCode3, countryDesc) VALUES (:country_value, :country_label)", array(':country_value' => $country->value, ':country_label' => $country->label));
          }
          watchdog('asu_rfi', "Cleared and sync'd RFI countries");
        }
        return true;
      }
    }
    return false;
  }

  public function getCountries(){
    $result = db_select('asu_rfi_countries', 'c')
      ->fields('c')
      ->execute()
      ->fetchAllAssoc("countryCode3");

    return $result;
  }

  /*
   * Handle Semester data
   */
  public function checkSemesterTableExists(){
    $val = db_table_exists("asu_rfi_sem_dates");
    return $val;
  }
  public function pullSemesters(){
    $type = 'select';
    $name = 'rfi_dedupe_list_terms';

    $data = $this->pullData($type, $name);

    if(empty($data)){
      watchdog('asu_rfi', "Failed to pull Semester data from the Middleware");
    }

    return $data;
  }

  public function syncSemesters(){

    if($this->checkSemesterTableExists()){
      $MWData = $this->pullSemesters();
      $localData = $this->getSemesters();
      if(!$MWData){
        watchdog('asu_rfi', "Failed to sync semesters");
      } else {
        $clean = true;
        foreach($MWData as $item){
          if(isset($localData[$item->Id])){
            if($localData[$item->Id]->semLabel !== $item->Name){
              $clean = false;
            }
          } else {
            $clean = false;
          }
        }
        if(!$clean){
          db_truncate('asu_rfi_sem_dates')->execute();
          foreach($MWData as $semester){
            db_query("INSERT INTO {asu_rfi_sem_dates} (semId, semLabel) VALUES (:sem_id, :sem_label)", array(':sem_id' => $semester->Id, ':sem_label' => $semester->Name));
          }
          watchdog('asu_rfi', "Cleared and sync'd RFI semesters");
        }
        return true;
      }
    }
    return false;
  }

  public function getSemesters(){
    $result = db_select('asu_rfi_sem_dates', 's')
      ->fields('s')
      ->execute()
      ->fetchAllAssoc("semId");

    return $result;
  }

  /*
   * Handle State data
   */
  public function checkStateTableExists(){
    $val = db_table_exists("asu_rfi_states_list");
    return $val;
  }
  public function pullStates(){
    $type = 'fieldinfo';
    $name = 'state_province';

    $data = $this->pullData($type, $name);

    if(empty($data)){
      watchdog('asu_rfi', "Failed to pull State data from the Middleware");
    }
    return $data;
  }
  public function syncStates(){
    if($this->checkStateTableExists()){
      $MWData = $this->pullStates();
      $localData = $this->getStates();
      if(!$MWData){
        watchdog('asu_rfi', "Failed to sync States");
      } else {
        $clean = true;
        foreach($MWData as $item){
          if(isset($localData[$item->value])){
            if($localData[$item->value]->stateDesc !== $item->label){
              $clean = false;
            }
          } else {
            $clean = false;
          }
        }
        if(!$clean){
          db_truncate('asu_rfi_states_list')->execute();
          foreach($MWData as $state){
            db_query("INSERT INTO {asu_rfi_states_list} (stateCode, stateDesc) VALUES (:state_value, :state_label)", array(':state_value' => $state->value, ':state_label' => $state->label));
          }
          watchdog('asu_rfi', "Cleared and sync'd RFI countries");
        }
        return true;
      }
    }
    return false;
  }
  public function getStates(){
    $result = db_select('asu_rfi_states_list', 's')
      ->fields('s')
      ->execute()
      ->fetchAllAssoc("stateCode");

    return $result;
  }

}
