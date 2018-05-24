function lxfEndtime(){
		$t=$('#Number1').html();
		if($t!=0){
			$('#timeSkip').html($t-1);
			$i=setTimeout("lxfEndtime()",1000);
			if( $('#timeSkip').html() == 0 ){
				window.open('/','_self');
			}
		}
	}
	lxfEndtime();
