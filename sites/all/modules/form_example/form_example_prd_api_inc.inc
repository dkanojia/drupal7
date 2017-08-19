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
  
  if(isset($_REQUEST['id']))
    $parent_cat_id = $_REQUEST['id'];
  else
    return;
  
  $taxnomy = taxonomy_term_load($parent_cat_id);
  // $output = taxonomy_get_tree($name->vid, 0, 3, 1);;
  // print "<pre>";
  // print_r($taxnomy);
  // exit;
  $vid = intval($taxnomy->vid);

  $tree = taxonomy_get_nested_tree($vid,3);
  $output =output_taxonomy_nested_tree($tree);
  
  // print "<pre>";
  // print_r($output);
  // exit;
  
  $json = array();
  $json = drupal_json_encode($output);
  // drupal_json_output($output);
  print_r($json) ; 
  drupal_exit();


}

function taxonomy_get_nested_tree($terms = array(), $max_depth = NULL, $parent = 0, $parents_index = array(), $depth = 0) {
  $return = [];
  if (is_int($terms)) {
    $terms = taxonomy_get_tree($terms);
  }

  foreach($terms as $term) {
    foreach($term->parents as $term_parent) {
      if ($term_parent == $parent) {
        $return[$term->tid] = $term;
      }
      else {
        $parents_index[$term_parent][$term->tid] = $term;
      }
    }
  }

  foreach($return as &$term) {
    if (isset($parents_index[$term->tid]) && (is_null($max_depth) || $depth < $max_depth)) {
      $term->children = taxonomy_get_nested_tree($parents_index[$term->tid], $max_depth, $term->tid, $parents_index, $depth + 1);
    }
  }

  return $return;
}

function output_taxonomy_nested_tree($tree) {
    if (count($tree)) {
        foreach ($tree as $term) {

            $result_output ['name'] = $term->name;
            $result_output ['description'] = $term->description;

            $name = $term_name;
            if ($term->children) {
                // $output [] = output_taxonomy_nested_tree($term->children);
                $result_output ['child'] = output_taxonomy_nested_tree($term->children);
            }

            $output [] = $result_output;
        }
    }
    return $output;
}