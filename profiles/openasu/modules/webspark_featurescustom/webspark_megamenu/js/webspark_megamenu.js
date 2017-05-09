(function ($) {
    Drupal.behaviors.ghostSlider = {
        attach: function (context, settings) {
            var navOffset = 0;
            //show hidden Mega Menu slider when needed
            $(window).scroll(function () {
                if ($('#navbar-administration'.length > 0)) {
                    navOffset = $('#navbar-bar').height() + $('#navbar-tray').height();
                    if (typeof($('#navbar-tray').attr('data-offset-left')) !== typeof undefined) {
                        navOffset = navOffset - $('#navbar-tray').height();
                    }
                }
                if ($('#ASUNavMenu').length && ($('#ASUNavMenu').offset().top - ($(window).scrollTop() + navOffset)) < 1 && $('.ghostSlider').length < 1) {
                    $('#ASUNavMenu').clone(true).prependTo(document.body).addClass('ghostSlider')
                        .css({"position": "fixed", "z-index": "1000", "width": "100%", "visibility": "visible"})
                        .removeAttr('id');
                } else if ($('#ASUNavMenu').length && ($('#ASUNavMenu').offset().top - ($(window).scrollTop() + navOffset)) >= 1) {
                    $('.ghostSlider').remove();
                }
            });
        }
    }
    Drupal.behaviors.webspark_megamenu_hidden = {
        attach: function (context, settings) {

            if ($('.tb-megamenu-block.tb-block.tb-megamenu-block').length) {
                $('.tb-megamenu-block.tb-block.tb-megamenu-block').closest('.tb-megamenu-row.row-fluid').addClass('hide-extra-padding');
                $('.tb-megamenu-block.tb-block.tb-megamenu-block').closest('.tb-megamenu-block.tb-block.tb-megamenu-block').addClass('adding-padding');
            }

        }
    }
    Drupal.behaviors.webspark_resize_menu = {
    attach: function (context, settings) {
        if($('#ASUNavMenu').length){
            window.resizeable = smartResize.construct('#ASUNavMenu>.container>div>.region',
                '.tb-megamenu-item.level-1.mega>a', jQuery('.block-tb-megamenu'),
                [{style: 'font-size: 15px; padding: 21px 10px 20px'},
                    {style: 'font-size:15px; padding: 20px 19px 21px;'},
                    {style: 'font-size:16px; padding: 20px 25px 21px;'}]);
        }
    }
}

  Drupal.behaviors.webspark_megamenu_driveify = {
    attach: function(context, settings){
      var rows = $('.tb-megamenu-row');
      try {
        for(var i=0; i<rows.length;i++){
          if(rows[i].children.length > 1){
            var some = rows[i].children;
            var bigheight = 0;
            for(var j = 0; j < some.length; j++){
              $('.tb-megamenu-submenu').css('display', 'block');
              if(some[j].className.indexOf("tb-megamenu-column") >= 0){
                if($('#' + some[j].id).height() > bigheight){
                  bigheight = $('#' + some[j].id).height();
                }
              }
              $('.tb-megamenu-submenu').css('display', '');
            }
            for(var j = 0; j < some.length; j++){
              $('#' + some[j].id).css('height', bigheight + 'px');
            }
          }
        }
      } catch (e){
      }
    }
  }
  Drupal.behaviors.webspark_megamenu = {
    attach: function() {
      var firstNavItem = $(".tb-megamenu-nav>li:first-child>a").first();
      try {
        if(firstNavItem.text().trim() == "Home"){
          var a = "<i class=\"fa fa-home icon-white\"><span style=\"display:none\">Home</span></i>";
          $(".tb-megamenu-nav>li:first-child>a").empty().append(a);
        } 
      } catch (e){
        
      }
    }
  }
}(jQuery));
