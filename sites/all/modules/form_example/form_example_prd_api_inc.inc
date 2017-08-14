<?php

/**
 * @file
 * Inc File for form in custom module.
 */

function _get_prd_api_json() {
  $elements = [];
  
  $elements['api_data'] = array(
    '#markup' => get_prd_api_jsondata(),
    // '#prefix' => '<div class="custom_form_main_container">',
    // '#suffix' => '</div>'
  );
  return $elements;
}

function _get_cat_api_json() {
  $elements = [];
  
  $elements['api_data'] = array(
    '#markup' => get_cat_api_jsondata(),
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

function get_prd_api_jsondata() {

  $cat_id_arr = [];
  
  $parent_cat_id = $_REQUEST['id'];
  $cat_arr = taxonomy_get_children($parent_cat_id);

  foreach ($cat_arr as $child_cat) {
    $cat_id_arr [] = $child_cat->tid;
    $child_cat_arr = taxonomy_get_children($child_cat->tid);

    foreach ($child_cat_arr as $sub_child_cat) {
      $cat_id_arr [] = $sub_child_cat->tid;
    }
  }

  $query = db_select('node','n');
  $query->leftjoin('field_data_field_category','fcat','fcat.entity_id=n.nid');

  $query->leftjoin('uc_products','uc_prd','uc_prd.nid=n.nid');

  $query->fields('n',array('title'));
  $query->fields('fcat');
  $query->fields('uc_prd',array('sell_price')); 
  $query->condition('fcat.field_category_tid ',$cat_id_arr,'IN');
  $query->condition('n.type','product');
  $result = $query->execute()->fetchAll();

  $res_conv_arr = cvf_convert_object_to_array($result);
  $json = array();
  $json = drupal_json_encode($res_conv_arr);
  drupal_json_output($json);  
  drupal_exit();
}

function get_cat_api_jsondata() {

  $cat_id_arr = [];
  
  // $parent_cat_id = '';

  if(isset($_REQUEST['id']))
    $parent_cat_id = $_REQUEST['id'];
  
  // $name = taxonomy_term_load($parent_cat_id);
  // $tax_vid = taxonomy_get_tree($name->vid,0, 2, 1);
  // print "<pre>";
  // print_r($tax_vid);
  // exit;

  // foreach ($tax_vid as $cat) {
  //   $cat_id_arr [] = $cat->tid;
  // }
  
  $d = taxonomy_get_tree(13, 40);
  $cat_id_arr = array();
  if (!empty($d)) {
    $cat_id_arr = _get_all_children_simplified($d);
  }

  $name = taxonomy_term_load(40);
  $output = taxonomy_get_tree($name->vid, 0, 3, 1);;
  
  $res_conv_arr = cvf_convert_object_to_array($output);
  $json = array();
  $json = drupal_json_encode($res_conv_arr);
  drupal_json_output($json);  
  drupal_exit();
}
