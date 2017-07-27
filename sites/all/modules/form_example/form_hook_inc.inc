<?php

function mymodule_menu()
{
  $menu['mycontent/add'] = array
  (
    'title' => 'Add new item',
    'page callback' => 'mymodule_add_item_page',
    'access callback' => TRUE,
  );
  $menu['mycontent/%mycontent'] = array
  (
    'title' => 'View item',
    'page callback' => 'mymodule_view_item_page',
    'page arguments' => array(1),
    'access callback' => TRUE,
  );
  $menu['mycontent/%mycontent/view'] = array
  (
    'title' => 'View',
    'access callback' => TRUE,
    'type' => MENU_DEFAULT_LOCAL_TASK,
  'weight' => -2,
  );
  $menu['mycontent/%mycontent/edit'] = array
  (
    'title' => 'Edit',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('mymodule_item_form', 1),
    'access arguments' => array('edit mymodule items'),
    'type' => MENU_LOCAL_TASK,
  'weight' => -1,
  );
  $menu['mycontent/%mycontent/delete'] = array
  (
    'title' => 'Delete',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('mymodule_item_delete_form', 1),
    'access arguments' => array('delete mymodule items'),
    'type' => MENU_LOCAL_TASK,
  );
  return $menu;
}

function mycontent_load($rid)
{
  return db_query('SELECT rid, my_field FROM {my_table} WHERE rid = :rid', array(':rid' => $rid))->fetchObject();
}

function mymodule_add_item_page()
{
  $item = new StdClass;
  $item->rid = FALSE;
  $item->my_field = '';

  return drupal_get_form('mymodule_item_form', $item);
}

function mymodule_item_form($form, &$form_state, $item)
{
  $form['#item'] = $item;
  $form['my_field'] = array
  (
    '#title' => t('Enter a value'),
    '#type' => 'textfield',
    '#default_value' => $item->my_field,
  );
  $form['submit'] = array
  (
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  return $form;
}

function mymodule_item_form_submit($form, &$form_state)
{
  $item = $form['#item'];
  if(isset($item->rid) && $item->rid)
  {
    db_update('my_table')
    ->fields(array('my_field' => $form_state['values']['my_field']))
    ->condition('rid', $item->rid)
    ->execute();
    $form_state['redirect'] = 'mycontent/' . $item->rid;
  }
  else
  {
    $rid = db_insert('my_table')
    ->fields(array('my_field' => $form_state['values']['my_field']))
    ->execute();
    $form_state['redirect'] = 'mycontent/' . $rid;
  }
}

function mymodule_view_item_page($item)
{
  $page['mymodule_item_page'] = array
  (
    '#markup' => t('The value of the item is @value', array('@value' => $item->my_field)),
    '#prefix' => '',
    '#suffix' => '',
  );

  return $page;
}

function mymodule_item_delete_form($form, &$form_state, $item)
{
  $form['#item'] = $item;
  // Note confirm_form() can be used here, but I prefer to use my own for styling purposes
  $form['header'] = array
  (
    '#markup' => t('Are you sure you wish to delete the item with a field value of @value?', array('@value' => $item->my_field)),
    '#prefix' => '',
    '#suffix' => '',
  );
  $form['warning'] = array
  (
    '#markup' => t('Warning, this action cannot be undone'),
  '#prefix' => '',
  '#suffix' => '',
  );
  $form['delete_button'] = array
  (
    '#type' => 'submit',
    '#value' => t('Delete item'),
  );
  return $form;
}

function mymodule_item_delete_form_submit($form, &$form_state)
{
  if($form_state['values']['op'] == $form_state['values']['delete_button'])
  {
    $item = $form['#item'];
    db_delete('my_table')
    ->condition('rid', $item->rid)
    ->execute();
  }
  $form_state['redirect'] = '';
  drupal_set_message(t('The item has been deleted'));
}