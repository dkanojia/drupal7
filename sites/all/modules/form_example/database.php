<?php
	$tables = array(
    			['und'] => array(
    				['User Table'] => array(
	                     
	                     'data_type' 	=> 'field_name',
	                     
	                     'int' 			=> 'id',
	                     'varchar' 		=> 'username',
	                     'varchar' 		=> 'password',
	                     'varchar' 		=> 'status',
	                     'varchar' 		=> 'scope',
	                     'varchar' 		=> 'type',
	                     'varchar' 		=> 'hash',
	                     'datetime' 	=> 'last_login_time',
	                     'tinyInt' 		=> 'created_date',
	                     ),

            		['Category Table'] => array(
	                     
	                     'data_type' 	=> 'field_name',
	                     
	                     'int' 			=> 'id',
	                     'varchar' 		=> 'name',
	                     'longtext' 	=> 'description',
	                     'int' 			=> 'parent_category_id',
	                     'varchar' 		=> 'custom_link',
	                     'varchar' 		=> 'cat_type',
	                     'int' 			=> 'category_image_id',
	                     'tinyInt' 		=> 'is_website_display',
	                     ),

            		['Category Product Association Table'] => array(
	                     
	                     'data_type' 	=> 'field_name',
	                     
	                     'int' 			=> 'id',
	                     'int' 			=> 'product_id',
	                     'int' 			=> 'category_id',
	                     ),

            		['Product Table'] => array(
	                     
	                     'data_type' 	=> 'field_name',

	                     'int' 			=> 'id',
	                     'varchar' 		=> 'name',
	                     'varchar' 		=> 'sku',
	                     'varchar' 		=> 'short_description',
	                     'text' 		=> 'description',
	                     'tinyInt' 		=> 'is_website_display',
	                     'datetime' 	=> 'created_date',
	                     'datetime' 	=> 'updated_date',
	                     ),

            		['Product Detail Table'] => array(
	                     
	                     'data_type' 	=> 'field_name',

	                     'int' 			=> 'id',
	                     'varchar' 		=> 'prod_color_code',
	                     'decimal' 		=> 'sale_price',
	                     'decimal' 		=> 'original_price',
	                     'int' 			=> 'currency_id',
	                     ),
            		// Detail Table will be customized in future also


            		['Product Image Table'] => array(
	                     
	                     'data_type' 	=> 'field_name',

	                     'int' 			=> 'id',
	                     'int' 			=> 'file_id',
	                     'int' 			=> 'product_id',
	                     'text' 		=> 'title',
	                     'text' 		=> 'alt_text',
	                     'int' 			=> 'sequence_no',
	                     ),
            		),



					['File Table'] => array(
	                     
	                     'data_type' 	=> 'field_name',

	                     'int' 			=> 'id',
	                     'int' 			=> 'related_id',
	                     'int' 			=> 'folder_id',
	                     'varchar' 		=> 'name',
	                     'longtext' 	=> 'content',
	                     'int' 			=> 'parent_id',
	                     'int' 			=> 'file_id',
	                     'int' 			=> 'created_by_id',
	                     'varchar' 		=> 'mime',
	                     'int' 			=> 'width',
	                     'int' 			=> 'height',
	                     'int' 			=> 'size',
	                     ),
            		),

					['File Sotre Table'] => array(
	                     
	                     'data_type' 	=> 'field_name',

	                     'int' 			=> 'id',
	                     'int' 			=> 'filestore_type_id',
	                     'int' 			=> 'filestore_volume_id',
	                     'varchar' 		=> 'original_filename',
	                     'varchar' 		=> 'filename',
	                     'int' 			=> 'filsize',
	                     'tinyInt' 		=> 'deleted',
	                     ),
            		),

					['File Store Image Table'] => array(
	                     
	                     'data_type' 	=> 'field_name',

	                     'int' 			=> 'id',
	                     'int' 			=> 'original_file_id',
	                     'int' 			=> 'thumb_file_id',
	                     ),
            		),

					['File Store Type Table'] => array(
	                     
	                     'data_type' 	=> 'field_name',

	                     'int' 			=> 'id',
	                     'varchar' 		=> 'name',
	                     'varchar' 		=> 'mime_type',
	                     'extension' 	=> 'extension',
	                     'tinyInt' 		=> 'allow',
	                     ),
            		),

					['File Store Volume Table'] => array(
	                     
	                     'data_type' 	=> 'field_name',

	                     'int' 			=> 'id',
	                     'varchar' 		=> 'name',
	                     'varchar' 		=> 'dirname',
	                     'bigint' 		=> 'total_space',
	                     'bigint' 		=> 'used_space',
	                     'tinyInt' 		=> 'is_volume_enabled',
	                     ),
            		),

					['Security Table'] => array(

	                     'data_type' 	=> 'field_name',

	                     'int' 			=> 'id',
	                     'int' 			=> 'user_ip',
	                     'varchar' 		=> 'country',
	                     'varchar' 		=> 'state',
	                     'varchar' 		=> 'city',
	                     'float' 		=> 'latitude',
	                     'float' 		=> 'longitude',
	                     ),
            		),

					
					['Currency Table'] => array(
	                     
	                     'data_type' 	=> 'field_name',

	                     'int' 			=> 'id',
	                     'varchar' 		=> 'icon',
	                     'varchar' 		=> 'name',
	                     'varchar' 		=> 'value',
	                     'varchar' 		=> 'integer_part',
	                     'varchar' 		=> 'fractional_part',
	                     'varchar' 		=> 'prefix',
	                     'varchar' 		=> 'postfix',
	                     'tinyInt' 		=> 'is_default_currency',
	                     ),
            		),
					
					// It will be break into two table and 
					// customize according type to manage enquiries
					['Enquiry Form Table'] => array(
	                     
	                     'data_type' 	=> 'field_name',

	                     'int' 			=> 'id',
	                     'varchar' 		=> 'name',
	                     'email' 		=> 'email',
	                     'varchar' 		=> 'subject',
	                     'varchar' 		=> 'description',
	                     'varchar' 		=> 'type' => array(
	                     					'Follow',
	                     					'New',
	                     					'Seen',
	                     					'Deleted',
	                     					'Completed',
	                     					'Discarded',
	                     					),
	                     'varchar' 		=> 'status',
	                     'datetime' 	=> 'enquiry_date',
	                     'datetime' 	=> 'enquiry_server_time',
	                     ),
            		),
    			);

?>