Drupal.behaviors.custom_module = {	
  attach: function (context, settings) {
    
    jQuery('.edit-form-detail').unbind('click').bind('click', function(ev) {
      ev.preventDefault();         
      var nid = jQuery(this).attr('data-value');  
      console.log(nid);  
      
      jQuery('#edit-nod-list').val(nid).trigger('change');   
      //alert(nid);      
    }); 
  
    // jQuery('.custom-user-delete').unbind('click').bind('click', function(e){ 
    //   return confirm('Are you sure, wanna to delete this user?');
    // });
  } 
}