<?php

/**
 * @file
 * Inc File for form in custom module.
 */

function _get_api_json($item) {
  $elements = [];
  
  $elements['api_data'] = array(
    '#markup' => get_api_jsondata($item),
    // '#prefix' => '<div class="custom_form_main_container">',
    // '#suffix' => '</div>'
  );
  return $elements;
}

function _get_api_json2() {
  $elements = [];
  
  $elements['api_data'] = array(
    '#markup' => get_api_jsondata2(),
    // '#prefix' => '<div class="custom_form_main_container">',
    // '#suffix' => '</div>'
  );
  return $elements;
}

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

function get_api_jsondata($item) {
  // drupal_set_message('My Node No :'.$newNode->nid);

  /* --------------------- For testing of apis to print the data in format of json ---------------*/
  $query = db_select('node','n');
  $query->leftjoin('field_data_body','frbd','frbd.entity_id=n.nid');
  $query->leftjoin('field_data_field_list_integer_','fl_int','fl_int.entity_id=n.nid');

  $query->leftjoin('field_data_field_image_testing','fim','fim.entity_id=n.nid');

  $query->leftjoin('file_managed','fman','fman.fid=fim.field_image_testing_fid');

  $query->fields('n',array('nid','title'));
  
  $query->fields('frbd',array('body_value'));
  $query->fields('fl_int',array('field_list_integer__value'));
  $query->fields('fim',array('field_image_testing_fid'));

  $query->fields('fman');

  $query->condition('n.nid',$item->nid);
  $query->condition('n.type','newsletter');

  // $query->condition('frb.field_radio_button_value',' ', '!=');
  // $query->condition('fim.field_image_fid',' ', '!=');
  
  $result = $query->execute()->fetchAll() ;

  $res_conv_arr = cvf_convert_object_to_array($result);

  // $file_url = file_create_url($res_conv_arr['0']['uri']);
  // $file_image_style_url = image_style_url("customstyle",$res_conv_arr['0']['uri']);
  
  // foreach ($res_conv_arr['0'] as $key => $value) {
    // drupal_set_message('Newly Node Result No - '.$newNode->nid.' Data : ' . $key . ':' . $value);
  // }
  
  // returnImagePath($file_url);
  // $result = $query->execute()->fetchAll();
  // $result = $query->execute()->fetchAllAssoc('nid');
  // $result = $query->execute()->fetchAllKeyed();
  // $result = $query->execute()->fetchAllKeyed(0,4);
  // $result = $query->execute()->fetchCol();
  // $result = $query->execute()->fetchCol(1);
  
  
  // $res_conv_arr = cvf_convert_object_to_array($result);
  
  // drupal_json_encode($result);
  $json = array();
  $json = drupal_json_encode($res_conv_arr);
  drupal_json_output($json);  
  // print_r($json);
  // exit;
  drupal_exit();

  // foreach ($res_conv_arr['0'] as $key => $value) {
  //   // drupal_set_message('Newly Node Result No - '.$newNode->nid.' Data : ' . $key . ':' . $value);
  // }

  // /*********-------------*********************/
  // // 2. Associated Array. 
  // // $result = $query->execute()->fetchAllAssoc('nid');
  // // $res_conv_arr = cvf_convert_object_to_array($result);

  // // foreach ($res_conv_arr[$newNode->nid] as $key => $value) {
  // //   drupal_set_message('Newly Node Result No - '.$newNode->nid.' Data : ' . $key . ':' . $value);
  // // }

  // /*********-------------*********************/
  // // 3. All Key without specific condition Array. 
  // // $result = $query->execute()->fetchAllKeyed();
  // // foreach ($result as $key => $value) {
  // //   drupal_set_message('Newly Node Result No - '.$newNode->nid.' Data : ' . $key . ':' . $value);
  // // }

  // /*********-------------*********************/
  // // 4. Datas in key pair
  // // $result = $query->execute()->fetchAllKeyed(0,4);

  // /*********-------------*********************/
  // // 5. Fetch Col Array. 
  // // $result = $query->execute()->fetchCol();
  
  // // foreach ($result as $key => $value) {
  // //   drupal_set_message('Newly Node Result No - '.$newNode->nid.' Data : ' . $key . ':' . $value);
  // // }

  // /*********-------------*********************/
  // // 6. Fetch Col Array. 
  // // (Based ON Fields Passing Array in fields function otherwise it takes first column)
  // // $result = $query->execute()->fetchCol();
  // // foreach ($result as $key => $value) {
  // //   drupal_set_message('Newly Node Result No - '.$newNode->nid.' Data : ' . $key . ':' . $value);

  // /*********-------------*********************/
  
  // /*********-------------*********************/

  // /* --------------------- For test the every function of drupal with mysql ---------------*/

  // // $res_conv_arr = cvf_convert_object_to_array($result);

  // $query = db_select('node','n');
  // $query->leftjoin('field_data_body','frbd','frbd.entity_id=n.nid');
  // $query->leftjoin('field_data_field_list_integer_','fl_int','fl_int.entity_id=n.nid');

  // $query->leftjoin('field_data_field_image_testing','fim','fim.entity_id=n.nid');

  // $query->leftjoin('file_managed','fman','fman.fid=fim.field_image_testing_fid');

  // // title
  // // body
  // // field_image_testing
  // // field_list_integer_
  // // field_date_iso_format_testing

  // $query->fields('n',array('nid','title'));
  
  // $query->fields('frbd',array('body_value'));
  // $query->fields('fl_int',array('field_list_integer__value'));
  // $query->fields('fim',array('field_image_testing_fid'));

  // $query->fields('fman');

  // $query->condition('n.nid',$newNode->nid);
  // $query->condition('n.type','newsletter');

  // // $query->condition('frb.field_radio_button_value',' ', '!=');
  // // $query->condition('fim.field_image_fid',' ', '!=');
  
  // $result = $query->execute()->fetchAll() ;

  // $res_conv_arr = cvf_convert_object_to_array($result);

  // $file_url = file_create_url($res_conv_arr['0']['uri']);
  // $file_image_style_url = image_style_url("customstyle",$res_conv_arr['0']['uri']);
  
  // foreach ($res_conv_arr['0'] as $key => $value) {
  //   drupal_set_message('Newly Node Result No - '.$newNode->nid.' Data : ' . $key . ':' . $value);
  // }
  
  // drupal_set_message('Newly Node No '. $newNode->nid .' Image Url : ' . $file_url);
  // drupal_set_message('Newly Node No '. $newNode->nid .' Image Style Url : ' . $file_image_style_url);

  // returnImagePath($file_url);
  // echo "<pre>";
  // print_r($result); 
  // echo "</pre>";
  // exit;

  // print_r(file_create_url('public://change-background/download_1.jpg'));

  // print "\n";
  // print image_style_url("50x50", 'public://change-background/download_1.jpg');

  // foreach ($res_conv_arr['0'] as $key => $value) {
  //   drupal_set_message('Newly Node Result No - '.$newNode->nid.' Data : ' . $key . ':' . $value);
  // }

}

function get_api_jsondata2() {

  // drupal_json_output($_REQUEST);
  // drupal_exit();
  
  $node_nid = $_REQUEST['id'];
  // drupal_set_message('My Node No :'.$newNode->nid);

  /* --------------------- For testing of apis to print the data in format of json ---------------*/
  $query = db_select('node','n');
  $query->leftjoin('field_data_body','frbd','frbd.entity_id=n.nid');
  $query->leftjoin('field_data_field_list_integer_','fl_int','fl_int.entity_id=n.nid');

  $query->leftjoin('field_data_field_image_testing','fim','fim.entity_id=n.nid');

  $query->leftjoin('file_managed','fman','fman.fid=fim.field_image_testing_fid');

  $query->fields('n',array('nid','title'));
  
  $query->fields('frbd',array('body_value'));
  $query->fields('fl_int',array('field_list_integer__value'));
  $query->fields('fim',array('field_image_testing_fid'));

  $query->fields('fman');

  $query->condition('n.nid',$node_nid);
  $query->condition('n.type','newsletter');

  $result = $query->execute()->fetchAll() ;

  $res_conv_arr = cvf_convert_object_to_array($result);

  // drupal_json_encode($result);
  $json = array();
  $json = drupal_json_encode($res_conv_arr);
  drupal_json_output($json);  
  // print_r($json);
  // exit;
  drupal_exit();
}