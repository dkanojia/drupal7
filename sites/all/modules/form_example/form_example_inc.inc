<?php

/**
 * @file
 * Inc File for form in custom module.
 */

function _get_rendering_element() {

  $elements = [];
  
  $getForm = drupal_get_form('form_example_form');

  $elements['render_form'] = array(
    '#markup' => drupal_render($getForm),
    // '#prefix' => '<div class="custom_form_main_container">',
    // '#suffix' => '</div>'
  );
  
  $elements['render_table'] = array(
    '#markup' => _get_form_posted_data(),
    '#prefix' => '<div class="custom_form_main_container">',
    '#suffix' => '</div>'
  );

  return $elements;
}

function form_example_form_custom_add ($form, &$form_state, $no_js = NULL) {

    // drupal_add_js('//translate.google.com/translate_a/element.js?cb=googleTranslateElementInit','external');
    // drupal_add_js('function googleTranslateElementInit() { new google.translate.TranslateElement({pageLanguage: "en",  multilanguagePage: true, layout: google.translate.TranslateElement.FloatPosition.BOTTOM_RIGHT},     "custom-user-data-listing-cust2")  }', 'inline');
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
    // '#process' => array('custom_module_user_form_process'), 
    '#process' => array('form_example_form'), 
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

function _get_form_posted_data() {
  $rows = array();
  $header = array(
    array('data'=>'Sr. No'),
    array('data'=>'Title'),
    array('data'=>'Photo'),
    array('data'=>'Edit Link'),
    array('data'=>'Delete Link'),
  );  
  
  $i = 1;

  $query = db_select('node','n');
  $query->fields('n');
  $query->condition('n.type','newsletter');

  $result = $query->execute()->fetchAll() ;

  $getNodeIds = array();
  foreach ($result as $res_arr) {
      $getNodeIds[] = $res_arr->nid;
  }

  foreach($getNodeIds as $delta => $nids) {
    $loadNodeData = node_load($nids);

    $delete_link =  '/mycontent/' . $nids .'/delete_slot';
    
    $edit_link =  '/mycontent/' . $nids .'/edit_slot';
    // $edit_link =  '/node-edit/' . $nids .'/edit_slot';

    if(isset($loadNodeData->field_image_testing['und']['0']['uri']))
      $img_url = file_create_url($loadNodeData->field_image_testing['und']['0']['uri']);
    else
      $img_url = "http://no_image";

    $rows[] = array(
      'data' => array(  
        array('data'=>$i),    
        array('data'=>$loadNodeData->title,'class'=> array('text-left')),        
        array('data'=>'<img src="'.$img_url.'" alt="photo" style="width:100px;height:125px">','class'=> array('text-left')),        
        array('data'=>l(t('Edit'),$edit_link, 
          array('attributes' => array
            (
              // 'target' => '_blank'
              'onclick' => 'if(!confirm("Really Edit?")){
                return false;
              }'
            ),
            // 'ajax' => array(
              // 'callback' => '_print_hello ',
              // 'wrapper' => 'channel_container_' . $channel->nid
            // )
          )
        )),        
        array('data'=>l(t('Delete'),$delete_link, 
          array('attributes' => array(
            'onclick' => 'if(!confirm("Really Delete?")){
              return false;
            }'
            ),
            'ajax' => array(
                'callback' => 'form_example_delete_node',
                'arguments' => $nids
              )
          )
        )),        
      ),  
    );
    
    $i++;
  }

  return theme('table', array('header'=>$header,'rows'=>$rows,'empty'=>'No records yet'));
}

// function form_example_form($form, &$form_state) {
function form_example_form($element, &$form_state) {
  
  if(isset($_GET['n_id'])){
    $nodeData = [];
    $nodeData = node_load($_GET['n_id']);  
  }
  /**
  Adding Css
    */
  drupal_add_css(drupal_get_path('module','form_example') . '/css/' . 'form_example.css');

  /**
  Adding Js
    */
  drupal_add_js(drupal_get_path('module', 'form_example') . '/js/' . 'form_example.js');
  /*
  Drupal Goto Function
  */
  // drupal_goto('test', array('query'=>array('variable_name1'=>'variable_value1)));
  
  $element['full-container']= array(
    '#type' => 'container',
    '#attributes' => array('class'=>array('col-md-12','col-sm-12','col-xs-12','project-details-left'),),
  );


  $form['#id'] = 'form_example';
  
  $form['hidden_nid'] = array(
    '#type' => 'hidden', 
    '#value' => isset($_GET['n_id']) ? $_GET['n_id'] : '',
  );

  $form['title'] = array(//you can find a list of available types in the form api
    '#type' => 'textfield', 
    '#title' => 'Name / Title',
    '#size' => 10,
    '#default_value' => isset($nodeData->title) ? $nodeData->title : ' ',
    '#maxlength' => 10,
    '#required' => TRUE, //make this field required 
  );

  $form['copy'] = array(  
    '#type' =>'checkbox', 
    '#title' => t('Send me a copy. (Testing Of checkbox)'),
    '#default_value' => isset($nodeData->field_boolean_on_off_check_box_['und']['0']['value']) ? $nodeData->field_boolean_on_off_check_box_['und']['0']['value'] : 0,
  );

  $form['high_school']['tests_taken'] = array(
    '#type' => 'checkboxes',
    '#options' => drupal_map_assoc(array(t('SAT'), t('ACT'))),
    '#title' => t('What standardized tests did you take? 
                  (Testing Of Multi CheckBoxes)'),
  );

  
  $currentDate = array('year' => 2017, 'month' => 2, 'day' => 15);

  if(isset($nodeData->field_date_iso_format_testing['und']['0']['value'])){
    $existing_date = $nodeData->field_date_iso_format_testing['und']['0']['value'];
    $format = 'Y-m-d';
    $existing_date = strtotime($existing_date);
    $final_date = date($format, $existing_date);
    $implode_date = explode('-', $final_date);

    $year = $implode_date['0'] ;
    $month = $implode_date['1'] ;
    $day = $implode_date['2'] ; 
    $currentDate = array('year' => $year, 'month' => $month, 'day' => $day);
  }

  $form['date_field'] = array(
      '#type' => 'date',
      '#title' => 'Choose Date (Testing of Date)',
      '#default_value' => $currentDate,
      // '#access' => FALSE,
  );

  $form['vertical_tabs_container'] = array(

    '#type' => 'vertical_tabs', 
    '#title' => t('Verical Tab Example (Testing Of Verical Tabs)'), 
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );


  $form['contact'] = array(

    '#type' => 'fieldset', 
    '#title' => t('Contact settings (Testing Of FieldSet)'), 
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#group' => 'vertical_tabs_container',
    '#description' => 'Contact setting description'.'<h1> hello </h1>'.
      '<ul>'
      // .' &lt;'.'li' .' &gt;'. ' style = "color: red;">Hello</li>
      .' &lt;'.'li'.' style = "color: red;"'.'&gt;'.'Hello'.' &lt;'.'li' .'&gt;'.'
      <li>Hello</li>
      <li>Hello</li>
      <li>Hello</li>
      </ul>',
  );

  $form['contact']['fieldset_text'] = array(

    '#type' => 'textfield', 
    '#title' => t('Field Set Text Settings (Testing Of FieldSet Text)'),
    '#description' => 'FieldSet Text',
  );

  $images = array(
    array(
      'fid' => 1, 
      'image_name' => '
        <table class="sticky-header" style="position: fixed; top: 29px; left: 56px; visibility: hidden;">
          <thead style="display: block;">
            <tr>
              <th>File information</th>
              <th class="tabledrag-hide" style="display: none;">Weight</th>
              <th>Operations</th> 
            </tr>
          </thead>
        </table>
        <table id="edit-field-image-testing-und-table" class="sticky-enabled tabledrag-processed tableheader-processed sticky-table">
          <thead>
            <tr>
              <th>File information</th>
              <th class="tabledrag-hide" style="display: none;">Weight</th>
              <th>Operations</th> 
            </tr>
          </thead>
          <tbody>
            <tr class="draggable odd">
              <td>
                <a href="#" class="tabledrag-handle" title="Drag to re-order">
                  <div class="handle">&nbsp;</div>
                </a>
                <div class="image-widget form-managed-file clearfix">
                  <div class="image-preview">
                    <img typeof="foaf:Image" src="http://localhost/drupal-7.56/sites/default/files/styles/thumbnail/public/image_example_images/screenshot_9.png?itok=G3fvcsfo" alt="" width="100" height="74">
                  </div>
                  <div class="image-widget-data">
                    <span class="file">
                      <img class="file-icon" alt="Image icon" title="image/png" src="/drupal-7.56/modules/file/icons/image-x-generic.png"> 
                        <a href="http://localhost/drupal-7.56/sites/default/files/image_example_images/screenshot_9.png" type="image/png; length=19658">screenshot.png
                        </a>
                    </span>  
                    <span class="file-size">(19.2 KB)</span> 
                    <input name="field_image_testing[und][0][fid]" value="224" type="hidden">
                    <input name="field_image_testing[und][0][display]" value="1" type="hidden">
                    <input name="field_image_testing[und][0][width]" value="294" type="hidden">
                    <input name="field_image_testing[und][0][height]" value="219" type="hidden">
                  </div>
                </div>
              </td>
              <td class="tabledrag-hide" style="display: none;">
                <div class="form-item form-type-select form-item-field-image-testing-und-0--weight">
                    <label class="element-invisible" for="edit-field-image-testing-und-0-weight">Weight for screenshot.png </label>
                    <select class="edit-field-image-testing-und-weight form-select" id="edit-field-image-testing-und-0-weight" name="field_image_testing[und][0][_weight]">
                      <option value="-4">-4</option>
                      <option value="-3">-3</option>
                      <option value="-2">-2</option>
                      <option value="-1">-1</option>
                      <option value="0" selected="selected">0</option>
                      <option value="1">1</option>
                      <option value="2">2</option>
                      <option value="3">3</option>
                      <option value="4">4</option>
                    </select>
                </div>
              </td>
              <td>
                <input id="edit-field-image-testing-und-0-remove-button" name="field_image_testing_und_0_remove_button" value="Remove" class="form-submit ajax-processed" type="submit">
              </td> 
            </tr>
          </tbody>
        </table>
      ',
      ),
  );

  $image_header = array(
    'image_name' => t('Image Name'),
    // 'last_name' => t('Last Name'),
  );

  $image_options = array();
  foreach ($images as $img) {
    $image_options[$img['fid']] = array(
      'image_name' => $img['image_name'],
      // 'last_name' => $user['last_name'],
    );
  }

  $form['image_table'] = array(
    '#type' => 'tableselect',
    '#header' => $image_header,
    '#options' => $image_options,
    '#empty' => t('No users found'),
  );  

  $form['file'] = array(
    '#type' => 'file',
    '#name' => 'files[]',
    '#title' => t('Choose multi file (Testing of file field)'),
    '#attributes' => array('multiple' => 'multiple'),
  );

  $form['machine_name'] = array(
    '#type' => 'machine_name',
    // '#default_value' => $vocabulary->machine_name,
    '#default_value' => 'machine_name',
    '#maxlength' => 21,
    // '#machine_name' => array(
    //   'exists' => 'menu_edit_menu_name_exists',
    // ),
  );

  $form['username'] = array(
    '#type' => 'textfield', //you can find a list of available types in the form api
    '#title' => 'User Name',
    '#size' => 10,
    '#maxlength' => 10,
    '#required' => TRUE, //make this field required 
  );

  $form['password_field'] = array(
    '#type' => 'password',
    '#title' => 'Set Password',
  );

  $form['pass'] = array(
    '#type' => 'password_confirm', 
    '#title' => t('Retype Pwd (Testing Password Confirm)'),
    '#size' => 25,
  );

  $form['radio_button'] = array(
    '#type' => 'radio',
    '#title' => 'Choose Default Option (Testing of Single Radio Button)',
    '#ajax[event]' => 'change',
    '#title_display' => 'after',
  );

  $admin = 'Admin';
  $active = array(0 => t('Closed'), 1 => t('Active'));
  $f = $form['settings']['active'] = array(
    '#type' => 'radios',
    '#title' => t('Poll status (Testing of Multi Radio Button)'),
    '#default_value' => isset($node->active) ? $node->active : 1,
    // '#default_value' => $node->active,
    '#options' => $active,
    '#description' => t('When a poll is closed, visitors can no longer vote for it.'),
    // '#access' => $admin,
  );

  $active2 = array('1' => t('1'), '5' => t('5'), '6' => t('6'), '9' => t('9'));
  $f2 = $form['settings2']['active2'] = array(
    '#type' => 'radios',
    '#title' => t('Poll status (Testing of Multi Radio Button For Integer)'),
    '#default_value' => isset($nodeData->field_list_integer_['und']['0']['value']) ? $nodeData->field_list_integer_['und']['0']['value'] : 1,
    // '#default_value' => $node->active,
    '#options' => $active2,
    '#description' => t('When a poll is closed, visitors can no longer vote for it.'),
    // '#access' => $admin,
  );

  $category = array('selected' => '0');
  $form['selected'] = array(
    '#type' => 'select',
    '#title' => t('Selected (Testing of Selected)'),
    '#options' => array(
        0 => t('No'),
        1 => t('Yes'),
    ),
   '#default_value' => $category['selected'],
   '#description' => t('Set this to <em>Yes</em> , if you would like this 
                        category to be selected by default.'),
  );

  $users = array(
    array('uid' => 1, 'first_name' => 'Indy', 'last_name' => 'Jones'),
    array('uid' => 2, 'first_name' => 'Darth', 'last_name' => 'Vader'),
    array('uid' => 3, 'first_name' => 'Super', 'last_name' => 'Man'),
  );

  $header = array(
    'first_name' => t('First Name'),
    'last_name' => t('Last Name'),
  );

  $options = array();
  foreach ($users as $user) {
    $options[$user['uid']] = array(
      'first_name' => $user['first_name'],
      'last_name' => $user['last_name'],
    );
  }

  $form['table'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
    '#empty' => t('No users found'),
  );

  if(isset($nodeData->body['und']['0']['value']))
    $body = $nodeData->body['und']['0']['value'];

  $form['description'] = array(
    '#type' => 'text_format',
    '#title' => t('Description (Testing of Text Format)'),
    '#default_value' => isset($body) ? $body : '',
  );

  $form['keywords'] = array(
    '#title' => t('Keywords (Testing of Text Area)'),
    '#type' => 'textarea',
    '#description' => t('The comment will be unpublished if it contains any of 
      the phrases above. Use a case-sensitive, comma-separated list of phrases. 
      Example: funny, bungee jumping, "Company, Inc."'),
    '#default_value' => isset(  $context['keywords']) ? drupal_implode_tags($context['keywords']) : '',
  );

  $form['price'] = array(
    '#type' => 'textfield', //you can find a list of available types in the form api
    '#title' => 'What is Your Price? (Testing of Text Field)',
    '#size' => 10,
    '#maxlength' => 10,
    '#required' => TRUE, //make this field required 
  );

  $edit = array('weight' => 0);
  $form['weight'] = array(
    '#type' => 'weight', 
    '#title' => t('Weight (Testing of weight to decide the order)'), 
    '#default_value' => $edit['weight'], 
    '#delta' => 1, 

    '#description' => t('Optional.
    In the menu, the heavier items will sink and the lighter items will be
    positioned nearer the top.'),
  );

  $common_arr = array('year' => 2017, 'month' => 2, 'day' => 15);
  $common_arr2 = 'hello';
  $form['testfield'] = array(
    '#type' => 'textfield', //you can find a list of available types in the form api
    '#title' => 'Test Field (with field prefix)',
    '#size' => 10,
    '#maxlength' => 10,
    // '#required' => TRUE, //make this field required 
    // '#field_prefix' => url(NULL, NULL, NULL, TRUE) . (variable_get('clean_url', 0) ? '' : '?q=')
    '#field_prefix' => url(NULL, $common_arr, NULL, TRUE) . (variable_get('clean_url', 0) ? '' : '?q=')
  );

  $multiple_form = array('1','2','3','4');

  foreach ($multiple_form as $key => $value) {
    $form['from1'.$value] = array(
      '#type' => 'item',

      '#title' => t('Item '.$value.' (Testing Of Item)'),

      '#markup' => 'Info! This is '.$value,
    );
  }

  // Special Elements 

  $form['preview'] = array(
    '#type' => 'button', 
    '#value' => t('Preview'), 
    // '#weight' => 19,
  );  

  // $elements = '1';
  // $field = array('type' => '1');
  $field_name = 'Container';

  // if ($elements) {
    // Also aid in theming of field widgets by rendering a classified
    // container.
    $form['container'] = array(
      '#type' => 'container',
      // '#attributes' => array(
        // 'class' => array(
          // 'field-type-' . drupal_html_class($field['type']),
          // 'field-name-' . drupal_html_class($field_name),
          // 'field-widget-' . drupal_html_class($instance['widget']['type']),
        // ),
      // ),
      // '#weight' => $instance['widget']['weight'],
    );
  // }

  $options = array('image' =>'http://localhost/drupal-7.56/themes/bartik/logo.png');
  $form['go'] = array(
      '#type' => 'image_button',
      '#src' => $options['image'],
      '#submit' => array('ctools_jump_menu_submit'),
      '#attributes' => array(
        'class' => array('ctools-jump-menu-button')
      )
    );

  $bid = '1'; 
  $form['bid'] = array(
    '#type' => 'hidden', 
    '#value' => $bid
  );

  $form['vid'] = array(
    '#type' => 'value', 
    '#value' => $bid
  );

  $form['actions'] = array('#type' => 'actions');
  
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit Form'),
  );

  $form['actions']['delete'] = array(
    '#type' => 'button',
    '#value' => t('Reset Form'),
  );

  $form['actions']['cancel'] = array(
    '#markup' => l(t('Cancel Form'), 'foo/bar'),
  );

  $status = array('3');
  $options = array('0','1','2','3','4');

  return $form;
}

function form_example_form_validate($form, &$form_state) {
  
  $num_files = count($_FILES['files']['name']);
    for ($i = 0; $i < $num_files; $i++) {
      $file = file_save_upload($i, array(
        'file_validate_is_image' => array(),
        'file_validate_extensions' => array('png gif jpg jpeg'),
      ));

      if ($file) {
        if ($file = file_move($file, 'public://image_example_images/')) {
          $form_state['values']['file'][$i] = $file;
        } else {
          form_set_error('file', t('Failed to write the uploaded file the site\'s file folder.'));
        }
      } else {
        form_set_error('file', t('No file was uploaded.'));
      }
    }
  }

//stdClass Object Converting to arrray;
function cvf_convert_object_to_array($data) {

    if (is_object($data)) {
        $data = get_object_vars($data);
    }

    if (is_array($data)) {
        return array_map(__FUNCTION__, $data);
    }
    else {
        return $data;
    }
}


function form_example_form_submit($form, &$form_state) {

  global $user;

  if(!isset($form_state['values']['hidden_nid'])){
    // add node properties
    $newNode = (object) NULL;
    $newNode->type = 'newsletter';
    $newNode->title = $form_state['values']['title'];
    $newNode->body['und']['0'] = $form_state['values']['description'];
    
    $newNode->field_boolean_on_off_check_box_['und']['0']['value'] = $form_state['values']['copy'];
    
    $newNode->field_list_integer_['und']['0']['value'] = $form_state['values']['active2'];
    
    $image_file_count = count($form_state['values']['file']);

    for($i=0; $i< $image_file_count; $i++){
      $newNode->field_image_testing['und'][] = (array)$form_state['values']['file'][$i];     
    }

    $year = $form_state['values']['date_field']['year'] ;
    $month = $form_state['values']['date_field']['month'] ;
    $day = $form_state['values']['date_field']['day'] ;
    $currentDate = $day."-".$month."-".$year ;
    $newNode->field_date_iso_format_testing['und']['0']['value'] = $currentDate ;
    
    $newNode->uid = $user->uid;
    $newNode->created = strtotime("now");
    $newNode->status = 1;
    $newNode->language = LANGUAGE_NONE;
    // $newNode->changed = strtotime("now");
    // $newNode->comment = 0;
    // $newNode->promote = 0;
    // $newNode->moderate = 0;
    // $newNode->sticky = 0;
    
    // exit;
    node_save($newNode);
    drupal_set_message('My Node No :'.$newNode->nid);
    
  }else{
    $node = [];
    $node = node_load($form_state['values']['hidden_nid']);
    // add node properties
    $node->title = $form_state['values']['title'];
    $node->body['und']['0'] = $form_state['values']['description'];

    $node->field_boolean_on_off_check_box_['und']['0']['value'] = $form_state['values']['copy'];
    $node->field_list_integer_['und']['0']['value'] = $form_state['values']['active2'];
    
    $year = $form_state['values']['date_field']['year'] ;
    $month = $form_state['values']['date_field']['month'] ;
    $day = $form_state['values']['date_field']['day'] ;
    $currentDate = $day."-".$month."-".$year ;
    $node->field_date_iso_format_testing['und']['0']['value'] = $currentDate ;

    $image_file_count = count($form_state['values']['file']);

    for($i=0; $i< $image_file_count; $i++){
      $node->field_image_testing['und'][] = (array)$form_state['values']['file'][$i];     
    }

    node_save($node);
    drupal_set_message('My Edited Node No :'.$node->nid);
  
    // $tableArray = array(
    //   'Category Table' => 'id
    //                       cat_name
    //                       cat_descr
    //                       parent_cat_id', 
      
    //   'Product Table' => 'id
    //                       cat_id
    //                       prod_name
    //                       prod_descr
    //                       prod_price
    //                       currency_id
    //                       ',

    //   'Currency Table' => 'id
    //                       curr_name
    //                       curr_value', 
    //   '' => '', 
    // );
  }
}