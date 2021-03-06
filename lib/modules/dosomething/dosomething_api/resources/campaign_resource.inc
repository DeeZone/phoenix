<?php

function _campaign_resource_definition() {
  $campaign_resource = [];
  $campaign_resource['campaigns'] = [
    'operations' => [

      'index' => [
        'help' => 'List all active campaigns.',
        'file' => [
          'type' => 'inc',
          'module' => 'dosomething_api',
          'name' => 'resources/campaign_resource'
        ],
        'callback' => '_campaign_resource_index',
        'args' => [
          [
            'name' => 'ids',
            'description' => 'Retrieve campaigns based on specific node ids.',
            'optional' => TRUE,
            'type' => 'string',
            'source' => ['param' => 'ids'],
            'default value' => NULL,
          ],
          [
            'name' => 'staff_pick',
            'description' => 'Retrieve campaigns that are staff picks.',
            'optional' => TRUE,
            'type' => 'boolean',
            'source' => ['param' => 'staff_pick'],
            'default value' => FALSE,
          ],
          [
            'name' => 'mobile_app',
            'description' => 'Flag to select campaigns available on the mobile app.',
            'optional' => TRUE,
            'type' => 'boolean',
            'source' => ['param' => 'mobile_app'],
            'default value' => FALSE,
          ],
          [
            'name' => 'mobile_app_date',
            'description' => 'Mobile app date to filter by.',
            'optional' => TRUE,
            'type' => 'string',
            'source' => ['param' => 'mobile_app_date'],
            'default value' => NULL,
          ],
          [
            'name' => 'term_ids',
            'description' => 'Taxonomy term ids to filter by.',
            'optional' => TRUE,
            'type' => 'string',
            'source' => ['param' => 'term_ids'],
            'default value' => NULL,
          ],
          [
            'name' => 'count',
            'description' => 'The number of campaigns to retrieve.',
            'optional' => TRUE,
            'type' => 'int',
            'source' => ['param' => 'count'],
            'default value' => 25,
          ],
          [
            'name' => 'random',
            'description' => 'Boolean to indicate whether to retrieve campaigns in random order.',
            'optional' => TRUE,
            'type' => 'boolean',
            'source' => ['param' => 'random'],
            'default value' => FALSE,
          ],
          [
            'name' => 'page',
            'description' => 'The zero-based index of the page to get, defaults to 0.',
            'optional' => TRUE,
            'type' => 'int',
            'source' => array('param' => 'page'),
            'default value' => 1,
          ],
          [
            'name' => 'cache',
            'description' => 'Boolean to indicate whether to cache data.',
            'optional' => TRUE,
            'type' => 'boolean',
            'source' => ['param' => 'cache'],
            'default value' => TRUE,
          ],
        ],
        'access callback' => '_campaign_resource_access_alt',
        'access arguments' => ['index'],
      ],

      'retrieve' => [
        'help' => 'Retrieve a specified campaign.',
        'file' => [
          'type' => 'inc',
          'module' => 'dosomething_api',
          'name' => 'resources/campaign_resource',
        ],
        'callback' => '_campaign_resource_retrieve',
        'args' => [
          [
            'name' => 'nid',
            'description' => 'The nid of the campaign to retrieve.',
            'optional' => FALSE,
            'type' => 'int',
            'source' => ['path' => 0],
          ],
        ],
        'access callback' => '_campaign_resource_access',
        'access arguments' => ['view'],
      ],

    ],


    // @TODO: The following relationships will be deprecated in the near future.
    'relationships' => array(
      'gallery' => array(
        'file' => array('type' => 'inc', 'module' => 'dosomething_api', 'name' => 'resources/campaign_resource'),
        'help'   => 'Returns Reportback Gallery. GET from campaigns/123/gallery',
        'access callback' => '_node_resource_access',
        'access arguments' => array('view'),
        'access arguments append' => TRUE,
        'callback' => '_campaign_resource_load_gallery',
        'args'     => array(
          array(
            'name' => 'nid',
            'optional' => FALSE,
            'source' => array('path' => 0),
            'type' => 'int',
            'description' => 'The nid of the node whose gallery we are getting',
          ),
          array(
            'name' => 'count',
            'type' => 'int',
            'description' => t('Number of Reportback Files to load.'),
            'source' => array('param' => 'count'),
            'optional' => TRUE,
          ),
          array(
            'name' => 'offset',
            'type' => 'int',
            'description' => t('If count is set to non-zero value, you can pass also non-zero value for start. For example to get Reportback Files from 5 to 15, pass count=10 and start=5.'),
            'source' => array('param' => 'offset'),
            'optional' => TRUE,
          ),
        ),
      ),
    ),
    'targeted_actions' => array(
      'signup' => array(
        'help' => 'Signup a user for a campaign. POST to campaigns/123/signup',
        'file' => array('type' => 'inc', 'module' => 'dosomething_api', 'name' => 'resources/campaign_resource'),
        'callback' => '_campaign_resource_signup',
        'access callback' => '_campaign_resource_access_alt',
        'access arguments' => array('signup'),
        'access arguments append' => TRUE,
        'args' => array(
          array(
            'name' => 'nid',
            'optional' => FALSE,
            'source' => array('path' => 0),
            'type' => 'int',
            'description' => 'The nid of the Campaign node to signup for.',
          ),
          array(
            'name' => 'values',
            'optional' => FALSE,
            'source' => 'data',
            'description' => 'The Signup data',
            'type' => 'array',
          ),
        ),
      ),
      'reportback' => array(
        'help' => 'Post a user reportback for a campaign: campaigns/123/reportback',
        'file' => array('type' => 'inc', 'module' => 'dosomething_api', 'name' => 'resources/campaign_resource'),
        'callback' => '_campaign_resource_reportback',
        'access callback' => '_campaign_resource_access_alt',
        'access arguments' => array('reportback'),
        'access arguments append' => TRUE,
        'args' => array(
          array(
            'name' => 'nid',
            'optional' => FALSE,
            'source' => array('path' => 0),
            'type' => 'int',
            'description' => 'The nid of the Campaign node to post a reportback.',
          ),
          array(
            'name' => 'values',
            'optional' => FALSE,
            'source' => 'data',
            'description' => 'The Reportback data',
            'type' => 'array',
          ),
        ),
      ),
    ),
  ];
  return $campaign_resource;
}


/**
 * Determine whether the current user can access a node resource.
 *
 * @param string $op
 *   Possible values of view, signup, reportback.
 * @param array $args
 *   Resource arguments passed through from the original request.
 * @return bool
 * @see node_access()
 */
// @TODO: Function will be deprecated and currently only used in soon to be deprecated targeted actions.
function _campaign_resource_access_alt($op = 'view', $args = array()) {
  if (DOSOMETHING_REPORTBACK_LOG) {
    watchdog('dosomething_api', '_campaign_resource_access_alt args:' . json_encode($args));
  }

  if ($op == 'index') {
    return TRUE;
  }

  $node = node_load($args[0]);
  if (!$node) {
    return services_error(t('No node found for @nid', array('@nid' => $args[0])), 403);
  }

  if ($op == 'view') {
    return node_access($op, $node);
  }

  if (!user_is_logged_in()) {
    return services_error(t('Must be logged in!'), 403);
  }

  if (dosomething_campaign_is_active($node)) {
    return TRUE;
    //@todo: If op==reportback and SMS Game, return 403 error.
  }

  return services_error(t('Campaign node @nid is not active.', array('@nid' => $node->nid)), 403);
}


/**
 * Determine whether the current user can access campaign resource.
 *
 * @param  string  $op
 * @return bool
 */
function _campaign_resource_access($op) {
  if ($op === 'view') {
    return TRUE;
  }

  if ($op === 'index') {
    return TRUE;
  }

  return FALSE;
}


/**
 * Retrieve and show response for campaign request.
 *
 * @param  string  $nid
 * @return object
 */
function _campaign_resource_retrieve($nid) {
  return (new CampaignTransformer)->show($nid);
}


/**
 * Retrieve and show index list response of campaigns requested.
 *
 * @param  string  $ids
 * @param  bool    $staff_pick
 * @param  bool    $mobile_app
 * @param  string  $mobile_app_date
 * @param  string  $term_ids
 * @param  int     $count
 * @param  bool    $random
 * @param  int     $page
 * @param  bool    $cache
 * @return object
 */
function _campaign_resource_index($ids, $staff_pick, $mobile_app, $mobile_app_date, $term_ids, $count, $random, $page, $cache) {
  $parameters = [
    'ids' => $ids,
    'staff_pick' => $staff_pick,
    'mobile_app' => $mobile_app,
    'mobile_app_date' => $mobile_app_date,
    'term_ids' => $term_ids,
    'count' => $count,
    'random' => $random,
    'page' => $page,
    'cache' => $cache,
  ];

  return (new CampaignTransformer)->index($parameters);
}


/**
 * Callback for Campaigns Signup targeted action.
 *
 * @param int $nid
 *   The Node nid to post the signup to.
 * @param array $values
 *   The signup data to post. Expected keys:
 *   - uid: The user uid (int).  Optional, uses global $user if not set.
 *   - source (string).
 */
function _campaign_resource_signup($nid, $values) {
  global $user;
  if (!isset($values['uid'])) {
    $values['uid'] = $user->uid;
  }
  if (!isset($values['source'])) {
    $values['source'] = NULL;
  }
  if (DOSOMETHING_SIGNUP_LOG_SIGNUPS) {
    watchdog('dosomething_api', '_campaign_resource_signup values:' . json_encode($values));
  }
  // @todo: Pass parameter into signup_create whether or not to send SMS.
  // Since SMS campaign signups would hit this endpoint, would not want
  // to send an additional "You've signed up text".
  return dosomething_signup_create($nid, $values['uid'], $values['source']);
}


/**
 * Callback for Campaigns Reportback targeted action.
 *
 * @param int $nid
 *   The Node nid to post the reportback to.
 * @param array $values
 *   The reportback data to post. Expected keys:
 *   - uid: The user uid (int).  Optional, uses global $user if not set.
 *   - file: Base64 encoded file string to save.
 *   - filename: The filename of the file provided as file.
 *   - file_url: The URL of the reportback file to save (used if no file/filename exist).
 *   - caption: The caption for the Reportback File if provided.
 *   - quantity (int).
 *   - why_participated (string).
 *   - num_participants (int).
 */
function _campaign_resource_reportback($nid, $values) {
  // @todo: Return error if signup doesn't exist.
  $values['nid'] = $nid;
  if (!isset($values['uid'])) {
    global $user;
    $values['uid'] = $user->uid;
  }
  $uid = $values['uid'];

  $file = NULL;

  if (!empty($values['file']) && !empty($values['filename'])) {
    $values['filepath'] = dosomething_reportback_get_file_dest($values['filename'], $nid, $uid);
    // Use Services module's File Create resource to save the file.
    module_load_include('inc', 'services', 'resources/file_resource');
    $result = _file_resource_create($values);
    $file = file_load($result['fid']);
  }
  if (!$file) {
    if (empty($values['file_url'])) {
      return services_error(t("Reportback file_url not found."), 500);
    }
    // Create a file from the $file_url.
    $file = dosomething_reportback_save_file_from_url($nid, $uid, $values['file_url']);
    if (!$file) {
      return services_error(t("Could not write file to destination"), 500);
    }
  }
  $values['fid'] = $file->fid;

  // @todo: Move this logic into dosomething_reportback_save().
  $rbid = dosomething_reportback_exists($nid, $uid);
  if (!$rbid) {
    $rbid = 0;
  }
  $values['rbid'] = $rbid;
  if (DOSOMETHING_REPORTBACK_LOG) {
    watchdog('dosomething_api', '_campaign_resource_reportback:' . json_encode($values));
  }
  return dosomething_reportback_save($values);
}


/**
 * Returns the Reportback Gallery of a specified node.
 *
 * @param int $nid
 *   Unique identifier for the node.
 * @param int $count
 *   Number of Reporback Files to return.
 * @param int $start
 *   Which RB File to start with. If present, $start and $count are used together
 *   to create a LIMIT clause to select RB Files. This could be used to do paging.
 * @return array
 *   An array of Reportback File objects.
 */
function _campaign_resource_load_gallery($nid, $count = 25, $start = 0) {
  $params = array(
    'nid' => $nid,
    'status' => 'approved',
  );
  return dosomething_api_get_reportback_files($params, $count, $start);
}
