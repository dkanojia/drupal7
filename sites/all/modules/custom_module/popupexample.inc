<?php
/**
 * Implements hook_menu()
 */
function custom_module_menu() {
 $items = array();
 $items['myform'] = array( 
  'title' => 'My form', //The name of the menu that will be displayed
  'page callback' => 'custom_module_page_callback', //The name of the function that will output a page
  'access callback' => TRUE,
  'type' => MENU_CALLBACK,
 );
 $items['mychart'] = array( 
  'title' => 'My chart', //The name of the menu that will be displayed
  'page callback' => 'custom_module_chart_page_callback', //The name of the function that will output a page
  'access callback' => TRUE,
  'type' => MENU_CALLBACK,
 );
 $items['custom/contact/add'] = array( 
  'title' => 'User Add Form', //The name of the menu that will be displayed
  'page callback' => 'custom_module_contact_page_callback', //The name of the function that will output a page
  'access callback' => TRUE,
  //'type' => MENU_CALLBACK,
 );
 $items['custom/contact/edit/%'] = array( 
  'title' => 'User Add Form', //The name of the menu that will be displayed
  'page callback' => 'custom_module_contact_page_callback', //The name of the function that will output a page
  'access callback' => TRUE,
  //'type' => MENU_CALLBACK,
 );
 $items['node-delete/%'] = array( 
  // 'title' => 'User Add Form', //The name of the menu that will be displayed
  'page callback' => 'custom_module_contact_delete_callback', //The name of the function that will output a page
  'page arguments' => array(1),
  'access callback' => TRUE,
  //'type' => MENU_CALLBACK,
 );
 $items['test-my-test-form'] = array(
   'title' => 'Drupal Form State',
   'page callback' => 'custom_module_test_form_callback',
   'access callback' => TRUE,
 );
 
 // 'edit/' . $node->nid . '/details'
 
 
 return $items;
}

function custom_module_page_callback() {
 //The argument is the name of the function with the form details
 return drupal_get_form('custom_module_form');
}

function custom_module_contact_page_callback() {
  $elements = array();
  $user_form = drupal_get_form('custom_module_user_add_form');
  $elements['custom_user_add_form'] = array(
    '#markup' => drupal_render($user_form),
    '#prifix' =>'<div class="clearfix margin-bottom-10" id="contatct-form-wrapper" style="margin-bottom: 20px;">',
    '#suffix' =>'</div>',
  );
  $elements['custom_user_listing'] = array(
    '#markup' => custom_get_user_table_data(),
    '#prefix' =>'<div class="clearfix margin-bottom-10"><div class="project-summary-listing" id="custom-user-data-listing-cust2">',
    '#suffix' =>'</div></div>',
  );
  
  return $elements;
   
}

function custom_module_test_form_callback() {
  return drupal_get_form('custom_modul_test_form');
}

function custom_module_user_add_form($form, &$form_state, $no_js = NULL) {
    drupal_add_js('//translate.google.com/translate_a/element.js?cb=googleTranslateElementInit','external');
    drupal_add_js('function googleTranslateElementInit() { new google.translate.TranslateElement({pageLanguage: "en",  multilanguagePage: true, layout: google.translate.TranslateElement.FloatPosition.BOTTOM_RIGHT},     "custom-user-data-listing-cust2")  }', 'inline');
  //$form['#attributes']['class'] = array('program_testing_form');
  if(empty($form_state['asset_cat_first_storage'])) {
      $tempIdentifer = uniqid();
      $form_state['asset_cat_first_storage'][]  = $tempIdentifer;
    }
   
  //$form_state['custom_contact_hobbey_storage'] = array();
  if(empty($form_state['custom_contact_hobbey_default_value'])) {    
    $form_state['custom_contact_hobbey_default_value'] = array();   
  }
  //dpm($form_state['triggering_element']);  
  if( !empty($form_state['triggering_element']['#name']) && $form_state['triggering_element']['#name'] == 'node-list' ) {    
    $triggeringElement = $form_state['triggering_element'];    
    $edit_id = $triggeringElement['#value'];      
    $edit_node = node_load($edit_id);  
    $form_state['asset_cat_first_storage'] = array();
    $form_state['custom_contact_hobbey_default_value'] = array();
    
    $hobbies = $edit_node->field_contact_hobies;
         
    foreach($hobbies['und'] as $hobby => $val) { 
        
      $tempIdentifer = uniqid();  
       
      $form_state['asset_cat_first_storage'][] = $tempIdentifer;
    
      $form_state['custom_contact_hobbey_default_value'][$tempIdentifer] = $val['value']; 
       
    } 
  }
      
  $form['main-container'] = array(
    '#type' => 'container',
    '#attributes' => array('class'=>array('row')), 
    '#prefix' => '<div class="clearfix margin-bottom-10"><div id="project_test_data_container" class="project-detail project_test_data_container">', 
    '#suffix' => '</div></div>',
    '#process' => array('custom_module_user_form_process'), 
  );
  $form['container-prefix'] = array(
    '#markup' => '<div id="main-container-multi-replace">',
  );
  $form['main-container-multi'] = array( 
    '#type' => 'container',
    '#attributes' => array('class'=>array('row')), 
    '#prefix' => '<div class="clearfix margin-bottom-10"><div id="asset-container-section" class="project-detail project_test_data_container alert-container-section">', 
    '#suffix' => '</div></div>',
    '#process' => array('custom_contact_multiple_hobbies_process'), 
  );
  $form['container-suffix'] = array(
    '#markup' => '</div>',
  );        
  $form['edit-nodeId'] = array(
    '#type' => 'hidden', 
    '#attributes' => array('id' => 'edit-nodeId'),
  );     
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'), 
    '#submit' => array('test_program_data_submit'),
    '#ajax' => array(      
      'callback' => 'custom_save_contact_data',       
      'wrapper' => 'custom-user-data-listing-cust',
      'method' => 'replace',
      'effect' => 'fade',       
    ), 
    '#prefix' => '<div class="pull-left custom-test-program margin-bottom-10">',
    '#suffix' => '</div>', 
  ); 
       
  return $form;
}

function custom_module_user_form_process($element, &$form_state) {
  $element['full-container']= array(
    '#type' => 'container',
    '#attributes' => array('class'=>array('col-md-12','col-sm-12','col-xs-12','project-details-left'),),
  );
  $query = db_select('node', 'n');
  $query->join('field_data_field_contact_name', 'na', 'n.nid = na.entity_id');  
  $query->fields('n',array('nid'));
  $query->fields('na',array('field_contact_name_value'));
  $result = $query->execute();
  $contact_list = array('' => 'None'); 
    while($row = $result->fetchAssoc()) {
       $contact_list[ $row['nid'] ]= $row['field_contact_name_value']; 
    }      
  $element['full-container']['node-list'] = array(
    '#type' => 'select',
    '#title' => t('Selected'),
    '#options' =>$contact_list, 
    '#prefix' =>'<div style="display:none;">',
    '#suffix' =>'</div>',
    '#ajax' => array( 
      'callback' => 'custom_contact_edit_form_callback',  
      'wrapper' => 'project_test_data_container',  
    ),
    '#attributes' => array('class'=> array('edit-custom-node-select')),
  );
    
  $element['full-container']['full_name'] = array( 
    '#type' => 'textfield', 
    '#title' => t('Name'), 
    '#default_value' => '',
    '#size' => 40,
    '#maxlength' => 128,
    '#required' => TRUE, 
  );
  $element['full-container']['email'] = array( 
    '#type' => 'textfield', 
    '#title' => t('Email'), 
    '#default_value' => '',
    '#size' => 40,
    '#maxlength' => 128, 
  );   
  $element['full-container']['address'] = array( 
    '#type' => 'textarea', 
    '#title' => t('Address'), 
    '#default_value' => '',        
  ); 
  
  return $element;  
}

function custom_contact_multiple_hobbies_process($element,&$form_state) {
  
  $storage = $form_state['asset_cat_first_storage'];
  $element['#storage'] = $storage;
  
  $element['fieldset_container'] = array(
      '#type' => 'fieldset',
	  '#title' => t('Your Hobbies'),
	  '#collapsible' => TRUE,
	  '#collapsed' => FALSE, 
  );
  
  foreach ($storage as $key => $uniqId) {    
    $element['fieldset_container']['test_name'][$uniqId] = array(
      '#type' => 'textfield',
      '#attributes'=> array('class'=> array('form-control','test_name','bg-yellow')),
      '#parents' => array('postedData',$uniqId,'test_name'),
	  '#prefix' => '<div class="pull-left">',
	  '#suffix' => '</div>',
    );
    
	$element['fieldset_container']['remove'][$uniqId] = array(
      '#type' => 'submit',
      '#value' => 'Remove',
      '#name' => $uniqId,
      '#attributes'=> array('style'=> array('font-family:FontAwesome;'),'class'=>array('alert-container-section', 'workflow-remove-submit')),
      '#parents' => array('postedData', $uniqId, 'remove'),      
      '#submit'=> array('remove_asset_category_handler'),
      '#ajax' => array(
        'callback' => 'asset_category_row_callback',
        'wrapper' => 'asset-container-section',
        'method' => 'replace',
        'effect' => 'fade',
        'progress' => array('type'=> 'throbber', 'message'=> NULL),
      ),	  
    );
    if(isset($form_state['custom_contact_hobbey_default_value'][$uniqId])) {
      $stdefault = $form_state['custom_contact_hobbey_default_value'][$uniqId];
      $element['fieldset_container']['test_name'][$uniqId]['#default_value'] = $stdefault;
    }
    //dpm($element['fieldset_container']);
   // drupal_set_message(print_r($form_state['custom_contact_hobbey_default_value'],1));
  }
  $element['workflow_add_new_row'] = array(
    '#type' => 'submit',
    '#value' => 'Add New',
    '#name' => 'workflow_add_new_row',
    '#attributes'=> array('class'=>array('routing-workflow-action','assets-workflow-action-add-new-assets'),'title'=>'Add New Detail'),
    '#submit'=> array('custom_multiple_hobbies_add_submit_callback'),
    '#ajax' => array(
      'callback' => 'custom_multiple_hobbies_add_ajax_callback',
      'wrapper' => 'asset-container-section',
      'method' => 'replace',
      'effect' => 'fade',
      'progress' => array('type'=> 'throbber', 'message'=> NULL),
    ),
    '#prefix' => '<div class="table-footer-custom clearfix margin-bottom-10">',
    '#suffix' => '</div>',
  );
  return $element;
}


function custom_save_contact_data($form, &$form_state) {
    $tempIdentifer = uniqid();
    $form_state['asset_cat_first_storage'] = array();
    $form_state['asset_cat_first_storage'][]  = $tempIdentifer;
    $commands = array();    
    $commands[] = ajax_command_invoke('#edit-full-name', 'val', array(''));
    $commands[] = ajax_command_invoke('#edit-email', 'val', array(''));
    $commands[] = ajax_command_invoke('#edit-address', 'val', array(''));
    $commands[] = ajax_command_html('#custom-user-data-listing-cust2', custom_get_user_table_data());
    $commands[] = ajax_command_html('#main-container-multi-replace', render($form['main-container-multi']));
    return array('#type' => 'ajax', '#commands' => $commands); 
}

/**
 *  
 * User delete callback
 * @param node_id  
 * 
 * */
 
function custom_module_contact_delete_callback($node_id) {
  if(!empty($node_id)) {      
    echo $node_id;//
    node_delete($node_id);
    drupal_set_message('User deleted successfully!');
    drupal_goto('/custom/contact/add');
      
  }    
}

function remove_asset_category_handler(&$form,&$form_state) {
  $triggering_element = $form_state['triggering_element'];
  $identifier = $triggering_element['#parents'][1];
  if(isset($form_state['asset_cat_first_storage'])){
    foreach ($form_state['asset_cat_first_storage'] as $key => $value) {
      if($form_state['asset_cat_first_storage'][$key] == $identifier){
        unset($form_state['asset_cat_first_storage'][$key]);
        unset($form_state['custom_contact_hobbey_default_value'][$identifier]);
      } 
    }             
  }   
  $form_state['rebuild'] = TRUE;
}

function asset_category_row_callback(&$form, &$form_state) {
  return $form['main-container-multi'];
}

function custom_multiple_hobbies_add_submit_callback(&$form, &$form_state) {
  $triggerElement = $form_state['triggering_element'];
  $tempIdentifer = uniqid();
  $form_state['asset_cat_first_storage'][]  = $tempIdentifer;    
  $form_state['rebuild'] = TRUE;
}

function custom_multiple_hobbies_add_ajax_callback(&$form, &$form_state) {  
  return $form['main-container-multi'];
}

 


/**
 * 
 *function for getting tables data. 
 * 
 **/ 
function custom_get_user_table_data() {
  drupal_add_js( drupal_get_path('module', 'custom_module') . '/js/form.js');
 // drupal_add_js(drupal_get_path('module', 'm6connect_hcfm') . '/m6connect_hcfm.js');
  $header = array(
    array('data'=>'Name','class'=>array('text-center col-md-2')),
    array('data'=>'Email','class'=>array('text-center col-md-2')),
    array('data'=>'Address','class'=>array('text-center col-md-3')),
    array('data'=>'Hobbies','class'=>array('text-center col-md-2')),
    array('data'=>'Action','class'=>array('text-center col-md-2')),
  );
  
  $nodes = node_load_multiple(array(), array('type' => 'contact'));
  $rows = array();
  $getResult = array();
  foreach($nodes as $node) { 
    $fname = !empty($node->field_contact_name['und'][0]['value']) ? $node->field_contact_name['und'][0]['value'] : 'Not available';
    $email = !empty($node->field_contact_email['und'][0]['value']) ? $node->field_contact_email['und'][0]['value'] : 'Not available';
    $address = !empty($node->field_contact_address['und'][0]['value']) ? $node->field_contact_address['und'][0]['value'] : 'Not available';
    $hobbies = $node->field_contact_hobies;
    
    $h = '';
    if( !empty($hobbies ) ) {
      foreach($hobbies['und'] as $key=> $val) {
         if( !empty($val['value'])) {
            $h .= $val['value']."<br>";
         } 
      }        
    } else {
        $h .= 'Not Available!';
    } 
    
    $action = '<div class="project-action">  
      <ul class="nav" aria-labelledby="dLabel"> 
        <li> '.l('Edit', 'javascript:void(0)', array('attributes' => array('class' => 'edit-commitment-detail', 'for' => $node->nid))).'</li>
        <li>'.l('Delete', url('node-delete/' . $node->nid), array( 'external'=>TRUE, 'attributes'=> array('class'=>array('custom-user-delete'))) ).'</li> </ul> ';
    
    $rows[] = array(
      'data' => array(
         array('data' => $fname, 'class' => array('text-left col-md-2')),
         array('data' => $email, 'class' => array('text-left col-md-2')),
         array('data' => $address, 'class' => array('text-left col-md-2'), 'style' => array('word-break:break-word;')),
         array('data' => $h, 'class' => array('text-left col-md-2')),
         array('data' => $action, 'class' => array('text-center col-md-2')),
      ), 
    );
  }
  $output = theme('table', array('header'=>$header,'rows'=>$rows,'empty'=>'No data yet','attributes'=>array('class'=>array('table','m6connect-custom-table','m6connect-custom-program-table','m6connect-program-commitment-table','table-hover','table-bordered','text-center'))));

return $output;
 
}
 
function test_program_data_submit(&$form, &$form_state) {
    
  $title = t('Contact');
  $name = $form_state['values']['full_name'];
  $email = $form_state['values']['email'];
  $address = $form_state['values']['address'];
  $hobbies = $form_state['values']['postedData'];
  $node_id = $form_state['values']['edit-nodeId'];
  //print '<pre>';
  //print_r($form_state['values']);die;
  $error = FALSE;    
    
  if (empty($name)) {
    form_set_error('full_name', 'Name can\'t be blank.');
    $error = TRUE;
  }
  if (empty($email)) {
    form_set_error('email', 'Email can\'t be blank.');
    $error = TRUE;
  }
     
  if($error){ return $form; }
  
//  dpm($node_id); die;
  if(!empty($node_id)) {
    
    $newnode = node_load($node_id);
   // dpm($newnode); die;
    //$newnode->type = "contact";
    $newnode->title = $name;
    $newnode->field_contact_name['und'][0]['value'] = $name;
    $newnode->field_contact_email['und'][0]['value'] = $email;  
    $newnode->field_contact_address['und'][0]['value'] = $address;  
    unset( $newnode->field_contact_hobies['und'] );
    
    foreach( $hobbies as $hobby => $val ) {
      $newnode->field_contact_hobies['und'][]['value'] = $val['test_name'];   
    } 
    
    node_save($newnode); 
    
    $form['#attached']['js'] = array(
       drupal_get_path('module', 'custom_module') . '/js/form.js',
    );

    $tempIdentifer = uniqid();
    $form_state['input'] = array();
    $form_state['asset_cat_first_storage'] = array();
    $form_state['asset_cat_first_storage'][] = $tempIdentifer;
    $form_state['rebuild'] = TRUE; 
    
  } else { 
    $newnode = new stdclass();
    $newnode->type = "contact";
    $newnode->title = $name;
    $newnode->field_contact_name['und'][0]['value'] = $name;
    $newnode->field_contact_email['und'][0]['value'] = $email;  
    $newnode->field_contact_address['und'][0]['value'] = $address; 
    foreach( $hobbies as $hobby => $val ) {
      $newnode->field_contact_hobies['und'][]['value'] = $val['test_name'];   
    }  
    node_save($newnode); 
    $tempIdentifer = uniqid();
    $form_state['input'] = array();
    $form_state['asset_cat_first_storage'] = array();
    $form_state['asset_cat_first_storage'][] = $tempIdentifer;
    $form_state['rebuild'] = TRUE;   
  }  
  
}

/**
 * 
 * funtion for edit form callback
 * 
 * */
 
function custom_contact_edit_form_callback($form, $form_state) { 
//  dpm($form_state);
  $triggeringElement = $form_state['triggering_element'];
  
  $form_state['custom_contact_hobbey_storage'] = array();
  
  $form_state['custom_contact_hobbey_default_value'] = array();
  
  $commands = array();
  
  if($form_state['triggering_element']['#type']=='select' && $form_state['triggering_element']['#name']=='node-list' ) { 
    
    $edit_id = $triggeringElement['#value'];  
    
    $edit_node = node_load($edit_id);   
    
    $commands[] = ajax_command_invoke('#edit-nodeId', 'val', array($edit_id));
    
    $commands[] = ajax_command_invoke('#edit-full-name', 'val', array($edit_node->field_contact_name['und'][0]['value']));
    
    $commands[] = ajax_command_invoke('#edit-email', 'val', array($edit_node->field_contact_email['und'][0]['value']));
    
    $commands[] = ajax_command_invoke('#edit-address', 'val', array($edit_node->field_contact_address['und'][0]['value'])); 
    
    $commands[] = ajax_command_replace('#asset-container-section', render($form['main-container-multi']));
    
    $commands[] = ajax_command_html('#custom-user-data-listing-cust2', custom_get_user_table_data());
  
  }  
  return array('#type' => 'ajax', '#commands' => $commands);
 
}

/*
function custom_module_form_alter(&$form, &$form_state, $form_id) {
   // dpm($form);
    if($form_id == 'article_node_form') {
       $form['field_image']['#prefix'] = "<div style='display: none;'>";
       $form['field_image']['#suffix'] = "</div>";
    }
      
}
*/
/*
function custom_module_user_presave(&$edit, $account, $category) { 
    if(isset( $_GET['gc'] ) && $_GET['gc'] == 'yes' ) {  
          if (isset($account->is_new) && !empty($account->is_new)) {
            $role = user_role_load_by_name('Employees');
            $edit['roles']= array($role->rid => 1);
          }
    }
    
}  */

function custom_modul_test_form($form, &$form_state) {
  $form['toogle_me'] = array(
    '#type' => 'checkbox',
    '#title' => t('Check me and see the magic!'),
  );  
  $form['setting_test'] = array(
    '#type' => 'textfield',
    '#title' => 'I will be visible if u uncheck that checkbox...',
    '#states' => array(
      'visible' => array(':input[name="toogle_me"]' => array('checked' => FALSE) ),
    ),
  );  
  
  return $form;
}


/**
 * 
 * Implements hook_block_info().
 * 
 * */
function custom_module_block_info(){
  $blocks = array();
  $blocks['my_custom_block_content'] = array(
    'info' => t('Custom Contact Block'),  
     
  );
  return $blocks;  
    
}

/**
 * 
 * Implements hook_block_view()
 * 
 * */
 
 function custom_module_block_view($delta = '') {
   $blocks = array(); 
   switch($delta) {
     case 'my_custom_block_content':
     $blocks['subject'] = '';
     $blocks['content'] = custom_block_return_content();
     break;
   }
   return $blocks; 
 }
 
 function custom_block_return_content(){
   $output = '';
   $output .= '<div class="well"><p class="alert alert-info">Hello this is my custom block! </p></div>';  
   $test_user_form = drupal_get_form('test_user_block_form');
   $output .= drupal_render($test_user_form); 
   return $output;
 }
 
 /**
  * 
  * Implements hook_block_configure()
  * 
  * */
  
  function custom_module_block_configure($delta = '') {
    $form = array();
    switch($delta){
      case 'my_custom_block_content':
        $form['text_body'] = array(
          '#type' => 'text_format',
          '#title' => t('Enter Your text here in WYSIWYG format'),
          '#default_value' => variable_get('text_variable', ''),
        );
        $form['file'] = array(
          '#name' => 'block_image',
          '#type' => 'managed_file',
          '#title' => t('Choose an image file'),
          '#description' => t('Select an image for custom block. Only .gif, .png, .jpg, .jpeg, Images allowed.'),
          '#default_value' => variable_get('block_image_fid', ''),
          '#upload_options' => 'public://block_image/',
          '#upload_validators' => array(
          'file_validate_extentions' => array('gif png jpg jpeg'),
          ), 
        );   
      break;  
    }
    
    return $form;
  }
  
/**
 * 
 * Implements hook_block_save()
 * 
 * */  
  
 function custom_module_block_save($delta = '', $edit = array()) {
   switch($delta) {
    case 'my_custom_block_content': 
      variable_set('text_variable', $edit['text_body']['value']); 
      $file = file_load($edit['file']);
      $file->status = FILE_STATUS_PERMANENT;
      file_save($file);
      $block = block_load('custom_module', $delta);
      file_usage_add($file, 'custom_module', 'block', $block->bid);
      variable_set('block_image_fid', $file->fid);
      break; 
   } 
 } 

function test_user_block_form($form, &$form_state) { 
   $form['Login_container'] = array(
     '#type' => 'container',
     '#attributes' => array(
       'class' => array('user-login','login-text'),
       'id' => 'user-contact-login',
     ),  
   );
   $form['Login_container']['username'] = array(
     '#type' => 'textfield',
     '#title' => t('Username'),
     '#size' => 60,
     '#maxlength' => 128,
     '#required' => TRUE,
   );
   $form['Login_container']['password'] = array(
     '#type' => 'password',
     '#title' => 'Password', 
     '#size' => 60,
     '#required' => TRUE,
   );
   $form['Login_container']['action'] = array(
     '#type' => 'submit',
     '#value' => 'Login',
     
   );
  return $form;  
}  
  