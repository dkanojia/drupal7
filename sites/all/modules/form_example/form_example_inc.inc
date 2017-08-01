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
  
  // $elements['custom_user_listing'] = array(
  //   '#markup' => custom_get_user_table_data(),
  //   '#prefix' =>'<div class="clearfix margin-bottom-10"><div class="project-summary-listing" id="custom-user-data-listing-cust2">',
  //   '#suffix' =>'</div></div>',
  // );

  $elements['render_table'] = array(
    '#markup' => _get_form_posted_data(),
    '#prefix' => '<div class="custom_form_main_container"> <div class="project-summary-listing" id="form-example-data-listing">',
    '#suffix' => '</div></div>'
  );

  return $elements;
}

function form_example_form($form, &$form_state, $no_js = NULL){

  if(empty($form_state['asset_cat_first_storage'])) {
      $tempIdentifer = uniqid();
      $form_state['asset_cat_first_storage'][]  = $tempIdentifer;
    }
   
  if(empty($form_state['form_example_form_subject_value'])) {    
    $form_state['form_example_form_subject_value'] = array();   
  }

  if( !empty($form_state['triggering_element']['#name']) && $form_state['triggering_element']['#name'] == 'nod-list' ) {    
    $triggeringElement = $form_state['triggering_element'];    
    $edit_id = $triggeringElement['#value'];      
    $edit_node = node_load($edit_id);  
    $form_state['asset_cat_first_storage'] = array();
    $form_state['form_example_form_subject_value'] = array();
    
    $subjects = $edit_node->field_subject;
         
    foreach($subjects['und'] as $subject => $val) { 
        
      $tempIdentifer = uniqid();  
       
      $form_state['asset_cat_first_storage'][] = $tempIdentifer;
    
      $form_state['form_example_form_subject_value'][$tempIdentifer] = $val['value']; 
       
    } 
  }
      
  $form['main-container'] = array(
    '#type' => 'container',
    '#attributes' => array('class'=>array('row')), 
    '#prefix' => '<div class="clearfix margin-bottom-10"><div id="project_test_data_container" class="project-detail project_test_data_container">', 
    '#suffix' => '</div></div>',
    '#process' => array('form_example_form_process'), 
  );

  // $form['image-container'] = array(
  //   '#type' => 'container',
  //   '#attributes' => array('class' => array('row')),
  //   '#prefix' => '<div class="clearfix margin-bottom-10"><div id="image_container_section" class="project-detail project_test_data_container">', 
  //   '#suffix' => '</div></div>',
  //   '#process' => array('form_example_image_form'),
  //   );

  $form['container-prefix'] = array(
    '#markup' => '<div id="main-container-multi-replace">',
  );

  $form['main-container-multi'] = array( 
    '#type' => 'container',
    '#attributes' => array('class'=>array('row')), 
    '#prefix' => '<div class="clearfix margin-bottom-10"><div id="asset-container-section" class="project-detail project_test_data_container alert-container-section">', 
    '#suffix' => '</div></div>',
    '#process' => array('form_example_multiple_subject_process'), 
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
    '#value' => t('Save Form Data'), 
    // '#submit' => array('test_program_data_submit'),
    '#submit' => array('form_example_form_submit'),
    '#ajax' => array(      
      // 'callback' => 'custom_save_contact_data',       
      'callback' => 'form_example_save_newsletter_data',       
      // 'wrapper' => 'custom-user-data-listing-cust',
      'wrapper' => 'form-example-data-listing',
      'method' => 'replace',
      'effect' => 'fade',       
    ), 
    '#prefix' => '<div class="pull-left custom-test-program margin-bottom-10">',
    '#suffix' => '</div>', 
  );

  return $form;

}

// function form_example_form_process($form, &$form_state) {
function form_example_form_process($element, &$form_state) {
  
  /**---------------*/
  // if(isset($_GET['n_id'])){
  //   $nodeData = [];
  //   $nodeData = node_load($_GET['n_id']);  
  // }
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
  
  // $form['#id'] = 'form_example';

  $element['full-container']= array(
    '#type' => 'container',
    '#attributes' => array('class'=>array('col-md-12','col-sm-12','col-xs-12','project-details-left'),),
  );
  
  $query = db_select('node', 'n');
  // $query->join('field_data_field_title', 'na', 'n.nid = na.entity_id');  
  $query->fields('n',array('nid','title'));
  // $query->condition('n.nid',' ','!=');
  $query->condition('n.type','newsletter');
  // $query->fields('na',array('field_title_value'));
  $result = $query->execute();
  $subject_list = array('' => 'None');
    while($row = $result->fetchAssoc()) {
       $subject_list[ $row['nid'] ]= $row['title']; 
    }      
  // print "<pre>";
  // print_r($subject_list);
  // exit; 
  
  $element['full-container']['nod-list'] = array(
    '#type' => 'select',
    '#title' => t('Selected'),
    '#options' =>$subject_list, 
    '#prefix' =>'<div style="display:none;">',
    '#suffix' =>'</div>',
    '#ajax' => array( 
      'callback' => 'form_example_edit_form_callback',  
      'wrapper' => 'project_test_data_container',  
    ),
    '#attributes' => array('class'=> array('edit-custom-node-select')),
    // '#attributes' => array('class'=> array('edit-custom-node-select form-select ajax-processed')),
  );
  
  $element['full-container']['hidden_nid'] = array(
    '#type' => 'hidden', 
    '#value' => isset($_GET['n_id']) ? $_GET['n_id'] : '',
  );

  $element['full-container']['title'] = array(//you can find a list of available types in the form api
    '#type' => 'textfield', 
    '#title' => 'Name / Title',
    '#size' => 10,
    // '#default_value' => isset($nodeData->title) ? $nodeData->title : ' ',
    '#maxlength' => 10,
    '#required' => TRUE, //make this field required 
  );

  $element['full-container']['copy'] = array(  
    '#type' =>'checkbox', 
    '#title' => t('Send me a copy. (Testing Of checkbox)'),
    // '#default_value' => isset($nodeData->field_boolean_on_off_check_box_['und']['0']['value']) ? $nodeData->field_boolean_on_off_check_box_['und']['0']['value'] : 0,
  );

  $element['full-container']['high_school']['tests_taken'] = array(
    '#type' => 'checkboxes',
    '#options' => drupal_map_assoc(array(t('SAT'), t('ACT'))),
    '#title' => t('What standardized tests did you take? 
                  (Testing Of Multi CheckBoxes)'),
  );

  
  $currentDate = array('year' => 2017, 'month' => 2, 'day' => 15);

  // if(isset($nodeData->field_date_iso_format_testing['und']['0']['value'])){
  //   $existing_date = $nodeData->field_date_iso_format_testing['und']['0']['value'];
  //   $format = 'Y-m-d';
  //   $existing_date = strtotime($existing_date);
  //   $final_date = date($format, $existing_date);
  //   $implode_date = explode('-', $final_date);

  //   $year = $implode_date['0'] ;
  //   $month = $implode_date['1'] ;
  //   $day = $implode_date['2'] ; 
  //   $currentDate = array('year' => $year, 'month' => $month, 'day' => $day);
  // }

  $element['full-container']['date_field'] = array(
      '#type' => 'date',
      '#title' => 'Choose Date (Testing of Date)',
      // '#default_value' => $currentDate,
      // '#access' => FALSE,
  );

  $element['full-container']['vertical_tabs_container'] = array(

    '#type' => 'vertical_tabs', 
    '#title' => t('Verical Tab Example (Testing Of Verical Tabs)'), 
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );


  $element['full-container']['contact'] = array(

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

  $element['full-container']['contact']['fieldset_text'] = array(

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

  $element['full-container']['image_table'] = array(
    '#type' => 'tableselect',
    '#header' => $image_header,
    '#options' => $image_options,
    '#empty' => t('No users found'),
  );  

  $edit_id = '';

  if( !empty($form_state['triggering_element']['#name']) && $form_state['triggering_element']['#name'] == 'nod-list' ) {    
    $triggeringElement = $form_state['triggering_element'];    

    if(isset($triggeringElement['#value']))
      $edit_id = $triggeringElement['#value'];
    else
      $edit_id = NULL;      
  }

  $element['full-container']['get_image_table'] = array(
    '#markup' => _get_image_table_data($edit_id),
    '#prefix' => '<div class="image_main_container"> <div class="image-listing" id="form-example-image-listing">',
    '#suffix' => '</div></div>'
  );

  $element['full-container']['file'] = array(
    '#type' => 'file',
    '#name' => 'files[]',
    '#title' => t('Choose multi file (Testing of file field)'),
    '#attributes' => array('multiple' => 'multiple'),
  );



  $element['full-container']['machine_name'] = array(
    '#type' => 'machine_name',
    // '#default_value' => $vocabulary->machine_name,
    '#default_value' => 'machine_name',
    '#maxlength' => 21,
    // '#machine_name' => array(
    //   'exists' => 'menu_edit_menu_name_exists',
    // ),
  );

  $element['full-container']['username'] = array(
    '#type' => 'textfield', //you can find a list of available types in the form api
    '#title' => 'User Name',
    '#size' => 10,
    '#maxlength' => 10,
    '#required' => TRUE, //make this field required 
  );

  $element['full-container']['password_field'] = array(
    '#type' => 'password',
    '#title' => 'Set Password',
  );

  $element['full-container']['pass'] = array(
    '#type' => 'password_confirm', 
    '#title' => t('Retype Pwd (Testing Password Confirm)'),
    '#size' => 25,
  );

  $element['full-container']['radio_button'] = array(
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
  $f2 = $element['full-container']['settings2']['active2'] = array(
    '#type' => 'radios',
    '#title' => t('Poll status (Testing of Multi Radio Button For Integer)'),
    // '#default_value' => isset($nodeData->field_list_integer_['und']['0']['value']) ? $nodeData->field_list_integer_['und']['0']['value'] : 1,
    // '#default_value' => $node->active,
    '#options' => $active2,
    '#attributes' => array('class' => array('form-custom-radio')),
    '#description' => t('When a poll is closed, visitors can no longer vote for it.'),
    // '#access' => $admin,
  );

  $category = array('selected' => '0');
  $element['full-container']['selected'] = array(
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

  $element['full-container']['table'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
    '#empty' => t('No users found'),
  );

  if(isset($nodeData->body['und']['0']['value']))
    $body = $nodeData->body['und']['0']['value'];

  $element['full-container']['description'] = array(
    '#type' => 'text_format',
    '#title' => t('Description (Testing of Text Format)'),
    // '#default_value' => isset($body) ? $body : '',
  );

  $element['full-container']['keywords'] = array(
    '#title' => t('Keywords (Testing of Text Area)'),
    '#type' => 'textarea',
    '#description' => t('The comment will be unpublished if it contains any of 
      the phrases above. Use a case-sensitive, comma-separated list of phrases. 
      Example: funny, bungee jumping, "Company, Inc."'),
    // '#default_value' => isset(  $context['keywords']) ? drupal_implode_tags($context['keywords']) : '',
  );

  $element['full-container']['price'] = array(
    '#type' => 'textfield', //you can find a list of available types in the form api
    '#title' => 'What is Your Price? (Testing of Text Field)',
    '#size' => 10,
    '#maxlength' => 10,
    '#required' => TRUE, //make this field required 
  );

  $edit = array('weight' => 0);
  $element['full-container']['weight'] = array(
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
  $element['full-container']['testfield'] = array(
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
    $element['full-container']['from1'.$value] = array(
      '#type' => 'item',

      '#title' => t('Item '.$value.' (Testing Of Item)'),

      '#markup' => 'Info! This is '.$value,
    );
  }

  // Special Elements 

  $element['full-container']['preview'] = array(
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
    // $element['full-container']['container'] = array(
    //   '#type' => 'container',
      // '#attributes' => array(
        // 'class' => array(
          // 'field-type-' . drupal_html_class($field['type']),
          // 'field-name-' . drupal_html_class($field_name),
          // 'field-widget-' . drupal_html_class($instance['widget']['type']),
        // ),
      // ),
      // '#weight' => $instance['widget']['weight'],
    // );
  // }

  $options = array('image' =>'http://localhost/drupal-7.56/themes/bartik/logo.png');
  $element['full-container']['go'] = array(
      '#type' => 'image_button',
      '#src' => $options['image'],
      '#submit' => array('ctools_jump_menu_submit'),
      '#attributes' => array(
        'class' => array('ctools-jump-menu-button')
      )
    );

  $bid = '1'; 
  $element['full-container']['bid'] = array(
    '#type' => 'hidden', 
    '#value' => $bid
  );

  $element['full-container']['vid'] = array(
    '#type' => 'value', 
    '#value' => $bid
  );

  // $element['full-container']['actions'] = array('#type' => 'actions');
  
  // $element['full-container']['actions']['submit'] = array(
  //   '#type' => 'submit',
  //   '#value' => t('Submit Form'),
  // );

  // $element['full-container']['actions']['delete'] = array(
  //   '#type' => 'button',
  //   '#value' => t('Reset Form'),
  // );

  // $element['full-container']['actions']['cancel'] = array(
  //   '#markup' => l(t('Cancel Form'), 'foo/bar'),
  // );

  $status = array('3');
  $options = array('0','1','2','3','4');

  return $element;
}

// function form_example_multiple_subject_process($element,&$form_state) {
function form_example_multiple_subject_process($element, &$form_state) {
  $storage = $form_state['asset_cat_first_storage'];

  $element['#storage'] = $storage;
  
  $element['fieldset_container'] = array(
      '#type' => 'fieldset',
    '#title' => t('Your Subject'),
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
      '#submit'=> array('remove_subject_asset_category_handler'),
      '#ajax' => array(
        'callback' => 'asset_category_row_callback_for_remove',
        'wrapper' => 'asset-container-section',
        'method' => 'replace',
        'effect' => 'fade',
        'progress' => array('type'=> 'throbber', 'message'=> NULL),
      ),    
    );
    
    if(isset($form_state['form_example_form_subject_value'][$uniqId])) {
      $stdefault = $form_state['form_example_form_subject_value'][$uniqId];
      $element['fieldset_container']['test_name'][$uniqId]['#default_value'] = $stdefault;
    }
    
    //dpm($element['fieldset_container']);
    // drupal_set_message(print_r($form_state['form_example_form_subject_value'],1));
  }

  $element['workflow_add_new_row'] = array(
    '#type' => 'submit',
    '#value' => 'Add New Subject',
    '#name' => 'workflow_add_new_row',
    '#attributes'=> array('class'=>array('routing-workflow-action','assets-workflow-action-add-new-assets'),'title'=>'Add New Detail'),
    '#submit'=> array('custom_multiple_subject_add_submit_callback'),
    '#ajax' => array(
      'callback' => 'custom_multiple_subject_add_ajax_callback',
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

function custom_multiple_subject_add_submit_callback($form, &$form_state) {

  $triggerElement = $form_state['triggering_element'];
  $tempIdentifer = uniqid();
  $form_state['asset_cat_first_storage'][]  = $tempIdentifer;    
  $form_state['rebuild'] = TRUE;
}

function custom_multiple_subject_add_ajax_callback(&$form, &$form_state) {  
  return $form['main-container-multi'];
}

// function remove_asset_category_handler(&$form,&$form_state) {
function remove_subject_asset_category_handler(&$form,&$form_state) {
  $triggering_element = $form_state['triggering_element'];

  $identifier = $triggering_element['#parents'][1];
  if(isset($form_state['asset_cat_first_storage'])){
    foreach ($form_state['asset_cat_first_storage'] as $key => $value) {
      if($form_state['asset_cat_first_storage'][$key] == $identifier){
        unset($form_state['asset_cat_first_storage'][$key]);
        unset($form_state['form_example_form_subject_value'][$identifier]);
      } 
    }             
  }   
  $form_state['rebuild'] = TRUE;
}

function asset_category_row_callback_for_remove(&$form, &$form_state) {
  return $form['main-container-multi'];
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

  $node_id = $form_state['values']['edit-nodeId'];
  global $user;

  // if(!isset($form_state['values']['hidden_nid'])){
  if(!empty($node_id)){

    $node = [];
    $node = node_load($node_id);
    // add node properties
    $node->title = $form_state['values']['title'];
    $node->body['und']['0'] = $form_state['values']['description'];

    $node->field_boolean_on_off_check_box_['und']['0']['value'] = $form_state['values']['copy'];
    $node->field_list_integer_['und']['0']['value'] = $form_state['values']['active2'];

    // $image_file_count = count($form_state['values']['file']);

    // for($i=0; $i< $image_file_count; $i++){
    //   $newNode->field_image_testing['und'][] = (array)$form_state['values']['file'][$i];     
    // }

    $subjects = $form_state['values']['postedData'];

    $count = count($node->field_subject['und']);

    $i = 0;
    foreach( $subjects as $subject => $val ) {

      if($i < $count)
        $node->field_subject['und'][$i]['value'] = $val['test_name'];
      else   
        $node->field_subject['und'][]['value'] = $val['test_name'];   
        
      $i++;
    }
    
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


    $form['#attached']['js'] = array(
       drupal_get_path('module', 'form_example') . '/js/form_alter.js',
    );

    $tempIdentifer = uniqid();
    $form_state['input'] = array();
    $form_state['asset_cat_first_storage'] = array();
    $form_state['asset_cat_first_storage'][] = $tempIdentifer;
    $form_state['rebuild'] = TRUE;

  }else{

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

    $subjects = $form_state['values']['postedData'];

    foreach( $subjects as $subject => $val ) {
      $newNode->field_subject['und'][]['value'] = $val['test_name'];   
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
    // drupal_set_message('My Node No :'.$newNode->nid);

    $tempIdentifer = uniqid();
    $form_state['input'] = array();
    $form_state['asset_cat_first_storage'] = array();
    $form_state['asset_cat_first_storage'][] = $tempIdentifer;
    $form_state['rebuild'] = TRUE;
  }
  
  //   // $tableArray = array(
  //   //   'Category Table' => 'id
  //   //                       cat_name
  //   //                       cat_descr
  //   //                       parent_cat_id', 
      
  //   //   'Product Table' => 'id
  //   //                       cat_id
  //   //                       prod_name
  //   //                       prod_descr
  //   //                       prod_price
  //   //                       currency_id
  //   //                       ',

  //   //   'Currency Table' => 'id
  //   //                       curr_name
  //   //                       curr_value', 
  //   //   '' => '', 
  //   // );
}

function form_example_save_newsletter_data($form, &$form_state) {
    $tempIdentifer = uniqid();
    $form_state['asset_cat_first_storage'] = array();
    $form_state['asset_cat_first_storage'][]  = $tempIdentifer;
    $commands = array();    
    $commands[] = ajax_command_invoke('#edit-title', 'val', array(''));
    $commands[] = ajax_command_invoke('#edit-price', 'val', array(''));
    $commands[] = ajax_command_invoke('#edit-description-value', 'val', array(''));

    $commands[] = ajax_command_html('#form-example-data-listing', _get_form_posted_data());
    $commands[] = ajax_command_html('#main-container-multi-replace', render($form['main-container-multi']));
    return array('#type' => 'ajax', '#commands' => $commands); 
}

function form_example_edit_form_callback($form, $form_state) {
  
  $triggeringElement = $form_state['triggering_element'];

  $form_state['form_example_subject_storage'] = array();
  
  $form_state['form_example_form_subject_value'] = array();
  
  $commands = array();
  
  if($form_state['triggering_element']['#type']=='select' && $form_state['triggering_element']['#name']=='nod-list' ) {

    $edit_id = $triggeringElement['#value'];  
    $edit_node = node_load($edit_id);   

    $commands[] = ajax_command_invoke('#edit-nodeId', 'val', array($edit_id));
    $commands[] = ajax_command_invoke('#edit-title', 'val', array($edit_node->title));
    $commands[] = ajax_command_invoke('#edit-price', 'val', array(''));
    $commands[] = ajax_command_invoke('#edit-description-value', 'val', array($edit_node->body['und'][0]['value']));

    $radio = $edit_node->field_list_integer_;
    $v = $radio['und'][0]['value'];

    // $commands[] = ajax_command_invoke('.vehicle_radios input[type="checkbox"]', 'attr',array('checked', 'true'));
    // $commands[] = ajax_command_invoke('.operation-done input[value="'.$item1['value'].'"]', 'attr',array('checked', 'true'));
    $commands[] = ajax_command_invoke('.form-custom-radio input[value="'.$v.'"]', 'attr',array('checked', 'true'));
    
    $commands[] = ajax_command_replace('#asset-container-section', render($form['main-container-multi']));
    // $commands[] = ajax_command_replace('#asset-container-section', render($form['main-container-multi']));

    $commands[] = ajax_command_html('#form-example-data-listing', _get_form_posted_data());
    $commands[] = ajax_command_html('#form-example-image-listing', _get_image_table_data($edit_id));
  }  
    // print "<pre>";
    // print_r($commands);
    // exit; 

  return array('#type' => 'ajax', '#commands' => $commands);

}

function form_example_delete_callback($node_id, $no_js = NULL) {
  ctools_include('ajax');
  $commands = array();
  node_delete($node_id);  
  $commands[] = ajax_command_html('#form-example-data-listing', _get_form_posted_data());
  // return array('#type' => 'ajax', '#commands' => $commands);
  print ajax_render($commands); // this function exits.
  exit;
  /*We can use both exit or return bcoz after rendring of html and echo of it, function should to be halt and return some value*/
  // return;
}


// function form_example_image_form(){
// function form_example_image_form($element, &$form_state){


//   $element['get_image_table'] = array(
//     '#markup' => _get_image_table_data(),
//     '#prefix' => '<div class="image_main_container"> <div class="image-listing" id="form-example-image-listing">',
//     '#suffix' => '</div></div>'
//   );

//   return $element;
  
// }

function _get_image_table_data($edit_id){
  if(!($edit_id == NULL)){
    drupal_add_js( drupal_get_path('module', 'form_example') . '/js/form_alter.js');

    $rows = array();
    $header = array(
      array('data'=>'Sr. No'),
      array('data'=>'File Detail'),
      array('data'=>'Remove Link'),
    );  
    // $edit_id = 103;
    // $nodeData = node_load(103);

    $nodeData = node_load($edit_id);
    $nid = $nodeData->nid;

    $image_arr = $nodeData->field_image_testing['und'];

    $i = 1;
    foreach ($image_arr as $key => $imag_uri_value) {

      
      $img_url = file_create_url($imag_uri_value['uri']);
      $img_name = $imag_uri_value['filename'];
      $img_id = $imag_uri_value['fid'];
      
      $remove_link = l('Remove', 
                      url('image-delete/' . $img_id.'/'. $nid .'/nojs'), 
                      array( 
                        'external'=>TRUE, 
                        'attributes'=> array(
                          'class'=>array('image-detail-delete','use-ajax')
                          )
                        )
                      );

      $rows[] = array(
        'data' => array(  
          array('data'=>$i),    
          array('data'=>'<div> <img src="'.$img_url.'" alt="photo" style="width:65px;height:65px"> </div> 
                        <div>'. $img_name .'</div>','class'=> array('text-left')),      
          array('data'=>$remove_link),
        ),  
      );
      
      $i++;

    }
    return theme('table', array('header'=>$header,'rows'=>$rows,'empty'=>'No records yet'));
  }else{
    return ;
  }

}

function form_example_image_delete_callback($file_id, $node_id, $no_js = NULL) {

  ctools_include('ajax');
  $commands = array();

  $node = node_load($node_id);
  $f_id = $file_id;

  $img_arr = $node->field_image_testing;

  // Extracting Position Of Image.
  foreach ($img_arr['und'] as $key => $value) {

    // Checking Position
    if($value['fid'] == $f_id){
      $pos_count = $und_inc;

      // Deleting entry from table
      // $file_delete_query = db_delete('field_data_field_image_testing')
      //       ->condition('entity_id', $node_id)
      //       ->condition('field_image_testing_fid', $value['fid']);

      // $file_delete_query->execute();

      // if($und_inc > 0)
      //   $und_inc--;
    
    }   

    // Updating table for delta position
    // $update_query = db_update('field_data_field_image_testing')
    //       ->fields(array('delta' => $und_inc))
    //       ->condition('entity_id', $node_id)
    //       ->condition('field_image_testing_fid', $value['fid'])
    //       ->condition('field_image_testing_fid', $f_id,'!=');
        
    // $update_query->execute();
    

    $und_inc++;
  }

  // $count = 1;

  // // Delete rows that have a exact or less value to prevent empty rows.
  // $file_del_query = db_delete('file_usage')
  //   ->condition('fid', $f_id)
  //   ->condition('id', $node_id);
  
  // if ($count) {
  //   $file_del_query->condition('count', $count, '<=');
  // }

  // $file_del_query->execute();


  // Flle loding and deleting with node updaing file
  $file = file_load($node->field_image_testing['und'][$pos_count]['fid']);
  unset($node->field_image_testing['und'][$pos_count]);
  node_save($node);

  file_delete($file);

  // If the row has more than the specified count decrement it by that number.
  // if (!$result && $count > 0) {
  //   $query = db_update('file_usage')
  //     ->condition('fid', $file->fid)
  //     ->condition('id', $node_id);
    
  //   $query->expression('count', 'count - :count', array(':count' => $count));
  //   $query->execute();
  // }

  $commands[] = ajax_command_html('#form-example-image-listing', _get_image_table_data($node_id));
  // return array('#type' => 'ajax', '#commands' => $commands);
  print ajax_render($commands); // this function exits.
  exit;

  /*We can use both exit or return bcoz after rendring of html and echo of it, function should to be halt and return some value*/
  // return;
}

function _get_form_posted_data() {

  drupal_add_js( drupal_get_path('module', 'form_example') . '/js/form_alter.js');

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
    
    $delete_link = l('Delete', 
                    url('node-delete/' . $nids.'/nojs'), 
                    array( 
                      'external'=>TRUE, 
                      'attributes'=> array(
                        'class'=>array('node-detail-delete','use-ajax')
                        )
                      )
                    );
    
    $edit_link = l('Edit', 
                  'javascript:void(0)', 
                  array(
                    'attributes' => array(
                      'class' => array(
                        'edit-form-detail'
                        ),
                      'data-value' => $nids 
                      )
                    )
                  );
    
    if(isset($loadNodeData->field_image_testing['und']['0']['uri']))
      $img_url = file_create_url($loadNodeData->field_image_testing['und']['0']['uri']);
    else
      $img_url = "http://no_image";

    $rows[] = array(
      'data' => array(  
        array('data'=>$i),    
        array('data'=>$loadNodeData->title,'class'=> array('text-left')),        
        array('data'=>'<img src="'.$img_url.'" alt="photo" style="width:100px;height:125px">','class'=> array('text-left')),      
        array('data'=>$edit_link),        
              // 'target' => '_blank'
              // 'onclick' => 'if(!confirm("Really Edit?")){
              //   return false;
              // }'
            // 'ajax' => array(
              // 'callback' => '_print_hello ',
              // 'wrapper' => 'channel_container_' . $channel->nid
            // )
        array('data'=>$delete_link),
          // array('attributes' => array(
          //     'onclick' => 'if(!confirm("Really Delete?")){
          //       return false;
          //     }'
          //     ),
          //     'ajax' => array(
          //         'callback' => 'form_example_delete_node',
          //         'arguments' => $nids
          //       )
          //   )
          // )
                  
      ),  
    );
    
    $i++;
  }

  return theme('table', array('header'=>$header,'rows'=>$rows,'empty'=>'No records yet'));
}

