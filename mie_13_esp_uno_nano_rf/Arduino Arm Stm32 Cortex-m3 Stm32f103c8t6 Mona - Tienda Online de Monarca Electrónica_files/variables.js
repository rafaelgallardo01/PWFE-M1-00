		"use strict";
		var digitech_brandnumber = 6,
			digitech_brandscrollnumber = 2,
			digitech_brandpause = 3000,
			digitech_brandanimate = 2000;
		var digitech_brandscroll = false;
							digitech_brandscroll = true;
					var digitech_categoriesnumber = 6,
			digitech_categoriesscrollnumber = 2,
			digitech_categoriespause = 3000,
			digitech_categoriesanimate = 700;
		var digitech_categoriesscroll = 'false';
					var digitech_blogpause = 3000,
			digitech_bloganimate = 2000;
		var digitech_blogscroll = false;
							digitech_blogscroll = false;
					var digitech_testipause = 3000,
			digitech_testianimate = 2000;
		var digitech_testiscroll = false;
							digitech_testiscroll = true;
					var digitech_catenumber = 6,
			digitech_catescrollnumber = 2,
			digitech_catepause = 3000,
			digitech_cateanimate = 700;
		var digitech_catescroll = false;
					var digitech_menu_number = 9;
		var digitech_sticky_header = false;
							digitech_sticky_header = true;
					jQuery(document).ready(function(){
			jQuery(".ws").on('focus', function(){
				if(jQuery(this).val()==""){
					jQuery(this).val("");
				}
			});
			jQuery(".ws").on('focusout', function(){
				if(jQuery(this).val()==""){
					jQuery(this).val("");
				}
			});
			jQuery(".wsearchsubmit").on('click', function(){
				if(jQuery("#ws").val()=="" || jQuery("#ws").val()==""){
					jQuery("#ws").focus();
					return false;
				}
			});
			jQuery(".search_input").on('focus', function(){
				if(jQuery(this).val()==""){
					jQuery(this).val("");
				}
			});
			jQuery(".search_input").on('focusout', function(){
				if(jQuery(this).val()==""){
					jQuery(this).val("");
				}
			});
			jQuery(".blogsearchsubmit").on('click', function(){
				if(jQuery("#search_input").val()=="" || jQuery("#search_input").val()==""){
					jQuery("#search_input").focus();
					return false;
				}
			});
		});
		