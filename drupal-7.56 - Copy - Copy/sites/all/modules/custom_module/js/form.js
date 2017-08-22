Drupal.behaviors.custom_module = {	
  attach: function (context, settings) {
    
    jQuery('.edit-commitment-detail').unbind('click').bind('click', function(ev) {
    ev.preventDefault();         
    var nid = jQuery(this).attr('for');  
    //console.log(nid);  
    jQuery('#edit-node-list').val(nid).trigger('change');   
    //alert(nid);      
  }); 
  
  jQuery('.custom-user-delete').unbind('click').bind('click', function(e){ 
    return confirm('Are you sure, wanna to delete this user?');
  });
  
  
  } 
   
  
}

/*
jQuery(document).ready(function() { 
 // alert("Hello");
  jQuery('.edit-commitment-detail').click(function(ev) {
    ev.preventDefault();         
    var nid = jQuery(this).attr('for');  
    //console.log(nid);  
    jQuery('#edit-node-list').val(nid).trigger('change');   
    //alert(nid);      
  }); 
  
  jQuery('.custom-user-delete').click(function(e){ 
    return confirm('Are you sure, wanna to delete this user?');
  });
   
});
*/