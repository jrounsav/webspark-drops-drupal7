<?php

/**
 * Class RFIEmailManager
 * Meant to handle emails from the RFI module to site managers as well as Requesters
 *
 * TODO support custom content
 * TODO Deal with duplicate emails
 */
class RFIEmailManager {

  public $from;
  public $to;
  public $subject;
  public $template;
  public $submission;
  public $body;

  public function __construct($submission = null, $internal = true)
  {
    if(!is_a($submission, "RFISubmission")){
      throw new Exception("The RFIEmailManager receiving an improper RFISubmission object.");
    }
    $this->submission = $submission;

    try {
      $this->setEmailRoute($internal);
      $this->body = $this->populateTemplate();
    } catch (Error $e) {
      watchdog("asu_rfi", $e);
    }

  }
  public function setEmailRoute($internal){
    switch ($internal){
      case TRUE:
        $this->from = variable_get('site_mail', ini_get('sendmail_from'));
        $this->to = variable_get('asu_rfi_error_email', NULL);
        $this->subject = "Failed submissions to Sales Force from " . $this->submission->getBaseURL();
        $this->setTemplate("asu_rfi_error.html");
        break;
      case FALSE:
        $this->from = !empty(variable_get('asu_rfi_from_email_field')) ? variable_get('asu_rfi_from_email_field') : 'asurecruitment@asu.edu';
        $this->to = $this->submission->getEmail();
        $this->subject = "Your requested ASU degree information.";
        $this->setTemplate();
        break;
      default:
        throw new Exception("RFIEmailManager can't tell whether the email is supposed to be sent internally or to a requester.");
        break;
    }
  }
  public function setTemplate($template = null){
    if(!empty($template)){
      $this->template = $template;
    } else {
      $level = $this->submission->getProspectLevel();
      switch ($level) {
        case "G":
          $this->template = "asu_rfi_grad_email.html";
          break;
        case "UG":
          $this->template = "asu_rfi_undergrad_email.html";
          break;
        default:
          throw new Exception("The RFIEmailManager doesn't know which email template to use.");
          break;
      }
    }
  }

  public function populateTemplate(){
    $body = "";
    $variables = array();
    $rfi_path = drupal_get_path("module", "asu_rfi");
    $template = file_get_contents($rfi_path . "/includes/templates/" . $this->template);
    $data = xmlrpc('https://webapp4.asu.edu/programs/XmlRpcServer', array('eAdvisorDSFind.findDegreeByAcadPlanMapArray' => array($this->submission->getProgram())));

    foreach ($data as $ball_arr) {
      $degree = $ball_arr["Descr100"];
      $degreeLevel = $ball_arr["Degree"];
      $desc = $ball_arr["DescrlongExtns"];
    }

    $variables['from_email'] = $this->from;
    $variables['name'] = !empty($this->submission->getFirstName()) ? $this->submission->getFirstName() : "" ;
    $variables['nodeid'] = !empty($this->submission->getNodeID()) ? $this->submission->getNodeID() : "";
    $variables['baseurl'] = !empty($this->submission->getBaseURL()) ?$this->submission->getBaseURL() : "";
    $variables['prog_plan'] = !empty($this->submission->getProgram()) ? $this->submission->getProgram() : "";
    $variables['degree'] =  !empty($degree) ? $degree : "";
    $variables['degreeLevel'] = !empty($degreeLevel) ? $degreeLevel : "";
    $variables['program_description'] = !empty($desc) ? "<strong><i> Program description</strong> </i><br />" . $desc : "";

    foreach($variables as $key => $value)
    {
      $template = str_replace('{{ '.$key.' }}', $value, $template);
    }

    return $template;

  }

  public function send(){
    try {
      // send the mail to the user
      $my_module = 'mimemail';
      $my_mail_token = '';
      $MimeMailSystem = drupal_mail_system($my_module, $my_mail_token);
      $message = array(
        'module' => 'mimemail',
        'id' => $my_module . '_' . $my_mail_token,
        'to' => $this->to,
        'from' => $this->from,
        'key' => '',
        'subject' => $this->subject,
        'body' => array($this->body),
        'headers' => array(
          'From' => $this->from,
          'Sender' => $this->from,
          'Return-Path' => $this->from,
          'Content-Type' => 'text/html; charset=UTF-8;',
          'to' => $this->to
        ),
      );

      $message = $MimeMailSystem->format($message);

      $result = $MimeMailSystem->mail($message);
    } catch (Exception $e) {
      watchdog('asu_rfi', 'An RFI function failed to send to generate an email: ' . $e->getMessage());
    }

    if (isset($result)) {
      return $result;
    } else {
      return false;
    }

  }
}
