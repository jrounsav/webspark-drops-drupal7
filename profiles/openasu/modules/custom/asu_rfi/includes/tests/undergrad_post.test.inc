<?php
require_once(__DIR__ . "/../asu_rfi_undergrad_post.inc");

class test extends PHPUnit_Framework_TestCase {
  public function test_asu_rfi_correct_campus_idu() {
    $this->assertEquals("TEMPE", asu_rfi_correct_campus_idu("Tempe"));
    $this->assertEquals("ONLNE", asu_rfi_correct_campus_idu("Online"));
    $this->assertEquals("EAC", asu_rfi_correct_campus_idu("East Arizona College"));
    $this->assertEquals("POLY", asu_rfi_correct_campus_idu("Polytechnic"));
    $this->assertEquals("CALHC", asu_rfi_correct_campus_idu("Lake Havasu"));
    $this->assertEquals("WEST", asu_rfi_correct_campus_idu("West"));
    $this->assertEquals("DTPHX", asu_rfi_correct_campus_idu("Downtown"));
    $this->assertEquals('', asu_rfi_correct_campus_idu(1));
    $this->assertEquals('', asu_rfi_correct_campus_idu(null));
  }
}