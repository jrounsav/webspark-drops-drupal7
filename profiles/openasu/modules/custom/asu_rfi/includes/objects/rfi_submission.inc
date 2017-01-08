<?php

class rfi_submission
{
  /**
   * Variables collected by environment
   */
  protected $source_id;
  protected $domain;
  protected $prod_test_flag;
  protected $attempted_submissions;
  protected $error_response;

  /**
   * Variables collected by form
   */
  protected $node_id;
  protected $ip_address;
  protected $type_of_form;
  protected $first_name;
  protected $last_name;
  protected $prospect_level;
  protected $student_type;
  protected $program;
  protected $starting_semester;
  protected $email;
  protected $zip;
  protected $phone;
  protected $birth_date;
  protected $country;
  protected $sms;
  protected $comments;
  protected $street;
  protected $city;
  protected $state;
  protected $intnl_student;
  protected $veteran_status;
  protected $campus;
  
  protected function setSourceId()
  {
    $domain = $this->getDomain();
    $source_id = $domain . '-' . $this->getTypeOfForm();
    $this->source_id = $source_id;
  }

  protected function getSourceId()
  {
    return $this->source_id;
  }

  protected function setDomain()
  {
    $default = $_SERVER['HTTP_HOST'];
    $domain = variable_get('asu_rfi_source_domain', $default);
    $this->domain = $domain;
  }

  protected function getDomain()
  {
    return $this->domain;
  }

  protected function setProdTestFlag()
  {
    $this->prod_test_flag = variable_get('asu_rfi_form_mode', 'test');
  }

  protected function getProdTestFlag()
  {
    return $this->prod_test_flag;
  }
  
  protected function setAttemptedSubmissions($attempted_submissions)
  {
    $this->attempted_submissions = $attempted_submissions;
  }
  
  protected function getAttemptedSubmissions()
  {
    return $this->attempted_submissions;
  }
  
  protected function setErrorResponse($error_response)
  {
    $this->error_response = $error_response;
  }
  
  protected function getErrorResponse()
  {
    return $this->error_response;
  }

  public function setTypeOfForm($type_of_form)
  {
    $this->type_of_form = $type_of_form;
  }

  public function getTypeOfForm()
  {
    return $this->type_of_form;
  }

  public function setNodeId($node_id)
  {
    $this->node_id = $node_id;
  }

  public function getNodeId()
  {
    return $this->node_id;
  }

  public function setIpAddress($ip_address)
  {
    $this->ip_address = $ip_address;
  }

  public function getIpAddress()
  {
    return $this->ip_address;
  }

  public function setFirstName($first_name)
  {
    $this->first_name = $first_name;
  }

  public function getFirstName()
  {
    return $this->first_name;
  }

  public function setLastName($last_name)
  {
    $this->last_name = $last_name;
  }

  public function getLastName()
  {
    return $this->last_name;
  }

  public function setProspectLevel($prospect_level)
  {
    $this->prospect_level = $prospect_level;
  }

  public function getProspectLevel()
  {
    return $this->prospect_level;
  }

  public function setStudentType($student_type)
  {
    $this->student_type = $student_type;
  }

  public function getStudentType()
  {
    return $this->student_type;
  }

  public function setProgram($program)
  {
    $this->program = $program;
  }

  public function getProgram()
  {
    return $this->program;
  }

  public function setStartingSemester($starting_semester)
  {
    $this->starting_semester = $starting_semester;
  }

  public function getStartingSemester()
  {
    return $this->starting_semester;
  }

  public function setEmail($email)
  {
    $this->email = $email;
  }

  public function getEmail()
  {
    return $this->email;
  }

  public function setZip($zip)
  {
    $this->zip = $zip;
  }

  public function getZip()
  {
    return $this->zip;
  }

  public function setPhone($phone)
  {
    $this->phone = $phone;
  }

  public function getPhone()
  {
    return $this->phone;
  }

  public function setBirthDate($birth_date)
  {
    $this->birth_date = $birth_date;
  }

  public function getBirthDate()
  {
    return $this->birth_date;
  }

  public function setCountry($country)
  {
    $this->country = $country;
  }

  public function getCountry()
  {
    return $this->country;
  }

  public function setSms($sms)
  {
    $this->sms = $sms;
  }

  public function getSms()
  {
    return $this->sms;
  }

  public function setComments($comments)
  {
    $this->comments = $comments;
  }

  public function getComments()
  {
    return $this->comments;
  }

  public function setStreet($street)
  {
    $this->street = $street;
  }

  public function getStreet()
  {
    return $this->street;
  }

  public function setCity($city)
  {
    $this->city = $city;
  }

  public function getCity()
{
  return $this->city;
}

  public function setState($state)
  {
    $this->state = $state;
  }

  public function getState()
  {
    return $this->state;
  }

  public function setIntnlStudent($intnl_student)
  {
    $this->intnl_student = $intnl_student;
  }

  public function getIntnlStudent()
  {
    return $this->intnl_student;
  }

  public function setVeteranStatus($veteran_status)
  {
    $this->veteran_status = $veteran_status;
  }

  public function getVeteranStatus()
  {
    return $this->veteran_status;
  }

  public function setCampus($campus)
  {
    $campus = $this->amend_campus($campus);
    $this->campus = $campus;
  }

  public function getCampus()
  {
    return $this->campus;
  }



  public function post()
  {
    watchdog('asu_rfi', 'There is no POST spec for default submission object. Must instantiate Undergraduate, Graduate, or Online submission object');
  }

  /**
   * Selects the College Code of the college offering a program.
   * @param $program string The program code of the selected program
   * @return mixed
   */
  protected function get_college_code($program)
  {
    $college_code = db_query('select field_asu_academic_program_value from field_data_field_asu_academic_program prog join field_data_field_asu_academic_plan plan on prog.entity_id = plan.entity_id where plan.field_asu_academic_plan_value = :plancode', array(':plancode' => $program))->fetchField();
    return $college_code;
  }

  /**
   * Changes the selected campus to it's campus code
   * @param $campus String The campus name
   * @return string
   */
  protected function amend_campus($campus)
  {

    switch ($campus)
    {
      case "Tempe":
        $campusId = "TEMPE";
        break;
      case "Online":
        $campusId = "ONLNE";
        break;
      case "East Arizona College":
        $campusId = "EAC";
        break;
      case "Polytechnic":
        $campusId = "POLY";
        break;
      case "Lake Havasu":
        $campusId = "CALHC";
        break;
      case "West":
        $campusId = "WEST";
        break;
      case "Downtown":
        $campusId = "DTPHX";
        break;
      default:
        $campusId = '';

    }

    return $campusId;
  }

  protected function report_error($response)
  {
    $error_message = $response;
    $this->setErrorResponse($error_message);
    $to_email = variable_get('asu_rfi_error_email', NULL); //get error reporting email address from rfi admin settings page
    //get middleware app key. If it's empty, don't send error email to admins.
    if (!empty($mw_auth_key)) {
      if (!empty($to_email)) {
        if (!isset($data['cron_run']) || ($data['cron_run'] != "Cron_running")) {
          $host = $_SERVER['HTTP_HOST'];
          //send email to admin reporting the issue
          $content = "The following <a href='https://$host/node/" . $this->getNodeId() . "'>submission</a> from " . $_SERVER['HTTP_HOST'] . " has some problem and hasn't been submitted to middleware";
          $subject = "Failed submissions to Sales Force from " . $_SERVER['HTTP_HOST'];
          $my_module = 'mimemail';
          $my_mail_token = '';
          $MimeMailSystem = drupal_mail_system($my_module, $my_mail_token);
          $default_from = variable_get('site_mail', ini_get('sendmail_from'));
          $message = array(
            'id' => $my_module . '_' . $my_mail_token,
            'from' => $default_from,
            'key' => '',
            'subject' => $subject,
            'body' => array($content),
            'to' => $to_email,
            'module' => 'mimemail',
            'headers' => array(
              'From' => $default_from,
              'Sender' => $default_from,
              'Return-Path' => $default_from,
              'Content-Type' => 'text/html; charset=UTF-8;',
              'to' => $to_email
            ),
          );

          $emailmessage = $MimeMailSystem->format($message);
          if ($MimeMailSystem->mail($emailmessage)) {
            watchdog('asu_rfi', 'SF Error message email sent to ' . $to_email);
            variable_set('SF_error_message_sent', time());
          }
        }
      }
    }

    if (drupal_valid_path('admin')) {
      drupal_set_message('Error message from Middleware: ' . $error_message);
    }
  }
}