"use strict";
// product-magnifier var
var digitech_magnifier_vars;
var yith_magnifier_options = {
		
		sliderOptions: {
			responsive: digitech_magnifier_vars.responsive,
			circular: digitech_magnifier_vars.circular,
			infinite: digitech_magnifier_vars.infinite,
			direction: 'left',
			debug: false,
			auto: false,
			align: 'left',
			height: 'auto', 
			//height: "100%", //turn vertical
			//width: 72,
			prev    : {
				button  : "#slider-prev",
				key     : "left"
			},
			next    : {
				button  : "#slider-next",
				key     : "right"
			},
			scroll : {
				items     : 1,
				pauseOnHover: true
			},
			items   : {
				visible: Number(digitech_magnifier_vars.visible),
			},
			swipe : {
				onTouch:    true,
				onMouse:    true
			},
			mousewheel : {
				items: 1
			}
		},
		
		showTitle: false,
		zoomWidth: digitech_magnifier_vars.zoomWidth,
		zoomHeight: digitech_magnifier_vars.zoomHeight,
		position: digitech_magnifier_vars.position,
		lensOpacity: digitech_magnifier_vars.lensOpacity,
		softFocus: digitech_magnifier_vars.softFocus,
		adjustY: 0,
		disableRightClick: false,
		phoneBehavior: digitech_magnifier_vars.phoneBehavior,
		loadingLabel: digitech_magnifier_vars.loadingLabel,
	};