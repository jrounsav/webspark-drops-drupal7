/**
 * Define the IS Text Widget (search input).
 */
(function ($) {

    AjaxSolr.asu_dirTextWidget = AjaxSolr.AbstractTextWidget.extend({

        constructor: function (attributes) {
            AjaxSolr.asu_dirTextWidget.__super__.constructor.apply(this, arguments);
            AjaxSolr.extend(this, {
                fieldConfigs: null,
                tree: null,
                fieldId: null
            }, attributes);
        },

        init: function () {
            var self = this;
            var fieldConfigs = this.fieldConfigs;
            var tree = this.tree;
            var fieldId = this.fieldId;


            //behavior for the search box.  Bind to enter key, and
            // add q value when there is one present
            $(this.target).find('input').bind('keydown', function (e) {

                if (e.which == 13) {

                    var fq = self.manager.store.values('fq');

                    var value = $(this).val();

                    if (value) {
                        self.manager.store.remove('q');
                        self.manager.store.addByValue('q', value);

                        // If sub-departments toggle is not selected, then add sub-departments for search,
                        // and remove other fq values
                        /*
                        if (!fieldConfigs.sub_toggle && fieldConfigs.show_tree) {
                            //remove only the deptids fq parameter
                            for (var i = 0; i < fq.length; i++) {
                                if (fq[i] != null && fq[i].indexOf("deptids:") != -1) {
                                    self.manager.store.removeByValue('fq', fq[i]);
                                }
                            }

                            var nid = ASUPeople[fieldId].dept_nid;
                            var sub_tree = asu_dir_ajax_solr_find_root(tree, nid);
                            var tree_ids = asu_dir_get_tree_ids(sub_tree);
                            var search_string = asu_dir_solr_search_string(tree_ids, 'deptids');

                            self.manager.store.addByValue('fq', search_string);
                        }*/

                        self.doRequest();
                    } else {
                        self.manager.store.remove('q');
                        self.manager.store.addByValue('q', '*:*');

                        /*
                        if (!fieldConfigs.sub_toggle && fieldConfigs.show_tree) {

                            //remove only the deptids parameter
                            for (var i = 0; i < fq.length; i++) {
                                if (fq[i].indexOf("deptids:") != -1) {
                                    self.manager.store.removeByValue('fq', fq[i]);
                                }
                            }
                            //Manager.store.remove('fq');
                            var nid = ASUPeople[fieldId].dept_nid;
                            self.manager.store.addByValue('fq', 'deptids:' + nid);
                        }*/
                        self.doRequest();
                    }
                }
            });
        },

        afterRequest: function () {
            var self = this;

            // If the persisted input field value and current q value match,
            // leave input value alone. Otherwise, clear it.
            if (self.manager.store.params.q.value == $(this.target).find('input').val()) {
                // Do nothing.
            }
            else {
                // Clear the value.
                $(this.target).find('input').val('');
            }
        } // end afterRequest

    });
})(jQuery);

