; Master make file for Webspark drush make process
; All of the modules' .make files contents have been copied into this file because Drush make won't 
; traverse subdirectories of a module for .make files.

; ASU Degrees Make file contents

api = 2
core = 7.x

projects[views_bootstrap][version] = 3.1
projects[views_bootstrap][type] = module
projects[views_bootstrap][subdir] = contrib

; ASU RFI make file contents

; Upload multidev and patch it

projects[multiblock][version] = 1.1
projects[multiblock][type] = module
projects[multiblock][subdir] = contrib
projects[multiblock][patch][] = https://www.drupal.org/files/issues/multiblock_cache-2185235-5.patch
projects[multiblock][patch][] = https://www.drupal.org/files/fix_config_per_instance-1370966-5.patch

; Added RFI dependencies here, vs. in openasu.make

projects[views_data_export][version] = 3.0-beta9
projects[views_data_export][type] = module
projects[views_data_export][subdir] = contrib
projects[views_data_export][sha256] = 004aab9b208fd8d4be8ac831e1c947d60b79233a37a785ef36f2991e450d81e3

projects[mimemail][version] = 1.0-beta4
projects[mimemail][type] = module
projects[mimemail][subdir] = contrib
projects[mimemail][sha256] = 28365ac72206125bc41c42b35832906d06bbdbb15ffb0433915eff013f2f111e

projects[honeypot][version] = 1.22
projects[honeypot][type] = module
projects[honeypot][subdir] = contrib
projects[honeypot][sha256] = 21c220ac6ee9b38b6c8f4c05f4e860361f626aee44a4b6ab29dfa190f9b504b6

; pre-req for mimemail
projects[mailsystem][version] = 2.34
projects[mailsystem][type] = module
projects[mailsystem][subdir] = contrib

;;;;;;;;;;;;;;;;;;;;;;
; Removed from Webspark openasu.make file
;;;;;;;;;;;;;;;;;;;;;;

; DEPRECATED PROJECTS - DO NOT USE AND LOOK FOR ALTERNATE SOLUTIONS ASAP

;; Will be replaced by alternate system that will work with the Hub's new asunews site

projects[asu_events][version] = 1.0-dev
projects[asu_events][subdir] = custom
projects[asu_events][type] = module
projects[asu_events][download][revision] = 3ff78bb9
projects[asu_events][download][url] = https://github.com/ASU/asu-drupal-events-feed.git

projects[asu_news][version] = 1.0-dev
projects[asu_news][subdir] = custom
projects[asu_news][type] = module
projects[asu_news][download][revision] = 7e925b539
projects[asu_news][download][url] = https://github.com/ASU/asu-drupal-news.git

;; (DEPRECATED in lieu of ASU Degrees - asu_degrees module)
projects[asu_eadvisor_degree_feed_feature][version] = 1.0-beta1
projects[asu_eadvisor_degree_feed_feature][subdir] = custom
projects[asu_eadvisor_degree_feed_feature][type] = module
projects[asu_eadvisor_degree_feed_feature][download][revision] = fbc26834
projects[asu_eadvisor_degree_feed_feature][download][url] = https://github.com/ASU/asu-drupal-eadvisor-degree-feed.git
