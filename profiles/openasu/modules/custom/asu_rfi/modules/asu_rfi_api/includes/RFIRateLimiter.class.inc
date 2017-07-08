<?php

/**
 * Class RFIRateLimiter verifies that the specified submissions per minute is not exceeded
 */
class RFIRateLimiter {

  /**
   * Check that the limit is not exceeded.
   * If not, allow the program to proceed.
   * If so, tell the program to stop.
   */
  public function checkLimit(){

    $response = array(
      'status' => 1,
      'response' => "Not over limit",
    );

    $time = time();
    $rate = $this->getRate();
    $inMin = $time - 60;

    $result = db_query("SELECT * FROM {asu_rfi_api_rate_limiter} WHERE created <= :time AND created >= :throttle", array(':time' => $time, ':throttle' => $inMin));

    $count = sizeof($result->fetchAllAssoc('id'));

    if($count < $rate){
      $this->proceed();
    } else {
      $response['status'] = 0;
      $response['response'] = "Rate limit exceeded";
    }

    return $response;

  }

  /**
   * Limiter has OK'd a request
   */
  public function proceed(){
    db_insert('asu_rfi_api_rate_limiter')
    ->fields(array(
      'created' => REQUEST_TIME,
    ))->execute();
  }

  /**
   * Returns the requests per minute
   * @return null
   */
  public function getRate(){
    return variable_get("asu_rfi_api_rpm", 20);
  }

  /**
   * Set the requests per minute value
   * @param $count
   * @return bool
   */
  public function setRate($count){
    $int = is_int($count);
    if($int){
      variable_set("asu_rfi_api_rpm", $count);
      return true;
    }
    watchdog("asu_rfi_api", "The specified max requests per minute is invalid");
    return false;
  }

}
