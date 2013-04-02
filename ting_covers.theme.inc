<?php

function template_preprocess_ting_object_cover(&$variables) {
  $object = $variables['object'] = $variables['elements']['#object'];
  $variables['image_style'] = $variables['elements']['#image_style'];

  $local_id = ting_cover_object_extract_id($object);
  //Set initial values required by the template
  $variables['classes'] = array('ting-cover',
                                'ting-cover-object-id-' . $local_id,
                                'ting-cover-style-' . $variables['image_style']);
  $variables['image'] = '';

  //If we already have the image available locally then just use it
  $path = ting_covers_object_path($local_id);
  if (file_exists($path)) {
    //Generate an alt tag
    $alt = implode(', ', $object->creators) . ': ' . $object->title;

    $variables['image'] = theme('image_style', array('style_name' => $variables['image_style'], 'path' => $path, 'alt' => $alt));

    //Avoid further javascript processing
    $variables['classes'][] = 'ting-cover-processed';
  }
  elseif (cache_get('ting_covers:' . $local_id)) {
    //We know that there is no cover available for this object so avoid
    //further javascript processing
    $variables['classes'][] = 'ting-cover-processed';
  }
}

function theme_ting_object_cover($variables) {
  return '<div class="' . implode(' ', $variables['classes']) . '">' . $variables['image'] . '</div>';
}

/**
 * Get the entity local id.
 *
 * This is useful since not always we have the local_id field value,
 * but we do the ding_entity_it.
 *
 * @param TingObject $entity
 *   The entity object whose is is expected.
 * @return String
 *   The objects identifier, whether in a numeric, or hash representation.
 */
function ting_cover_object_extract_id($entity) {
  $local_id = isset($entity->localId) ? $entity->localId : '';

  if (empty($local_id)) {
    $ding_id = $entity->ding_entity_id;
    $local_id = explode(':', $ding_id);
    $local_id = !empty($local_id[1]) ? $local_id[1] : 0;
  }

  return $local_id;
}
