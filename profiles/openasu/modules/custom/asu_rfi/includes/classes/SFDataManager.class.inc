<?php

/**
 * Class SFDataManager
 *
 * Link the use of data provided by Salesforce, to the storage in Drupal.
 */
class SFDataManager {

  private $dataTables = [];

  public function syncData(){}

  public function checkTableExists(){
    $val = mysql_query('select 1 from `table_name` LIMIT 1');

    if($val !== FALSE)
    {
      //DO SOMETHING! IT EXISTS!
    }
    else
    {
      //I can't find it...
    }
  }
  public function syncCountries(){}
  public function getCountries(){}

  public function checkSemestersExists(){}
  public function syncSemesters(){}
  public function getSemesters(){}

  public function checkStatesExists(){}
  public function syncStates(){}
  public function getStates(){}
}
