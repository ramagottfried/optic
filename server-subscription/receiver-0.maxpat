{
	"patcher" : 	{
		"fileversion" : 1,
		"appversion" : 		{
			"major" : 7,
			"minor" : 0,
			"revision" : 5,
			"architecture" : "x86",
			"modernui" : 1
		}
,
		"rect" : [ 372.0, 116.0, 846.0, 481.0 ],
		"bglocked" : 0,
		"openinpresentation" : 0,
		"default_fontsize" : 12.0,
		"default_fontface" : 0,
		"default_fontname" : "Arial",
		"gridonopen" : 1,
		"gridsize" : [ 15.0, 15.0 ],
		"gridsnaponopen" : 1,
		"objectsnaponopen" : 1,
		"statusbarvisible" : 2,
		"toolbarvisible" : 1,
		"lefttoolbarpinned" : 0,
		"toptoolbarpinned" : 0,
		"righttoolbarpinned" : 0,
		"bottomtoolbarpinned" : 0,
		"toolbars_unpinned_last_save" : 15,
		"tallnewobj" : 0,
		"boxanimatetime" : 200,
		"enablehscroll" : 1,
		"enablevscroll" : 1,
		"devicewidth" : 0.0,
		"description" : "",
		"digest" : "",
		"tags" : "",
		"style" : "",
		"subpatcher_template" : "no-menus",
		"boxes" : [ 			{
				"box" : 				{
					"fontface" : 0,
					"fontsize" : 12.0,
					"id" : "obj-13",
					"maxclass" : "o.display",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 564.0, 103.0, 185.0, 34.0 ],
					"text" : "/notice : \"disconnected\"",
					"textcolor" : [ 1.0, 1.0, 1.0, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-10",
					"maxclass" : "toggle",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "int" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 233.0, 297.0, 24.0, 24.0 ],
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-8",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "bang" ],
					"patching_rect" : [ 233.0, 333.0, 71.0, 22.0 ],
					"style" : "",
					"text" : "metro 5000"
				}

			}
, 			{
				"box" : 				{
					"fontface" : 0,
					"fontsize" : 12.0,
					"id" : "obj-7",
					"linecount" : 2,
					"maxclass" : "o.display",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 523.0, 222.0, 131.0, 48.0 ],
					"text" : "/foo : 28,\n/bar : 0.028",
					"textcolor" : [ 1.0, 1.0, 1.0, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-6",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 3,
					"outlettype" : [ "", "", "FullPacket" ],
					"patching_rect" : [ 482.0, 61.0, 101.0, 22.0 ],
					"style" : "",
					"text" : "o.route /api /data"
				}

			}
, 			{
				"box" : 				{
					"fontface" : 0,
					"fontsize" : 12.0,
					"id" : "obj-5",
					"linecount" : 2,
					"maxclass" : "o.compose",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 233.0, 375.0, 210.0, 38.0 ],
					"saved_bundle_data" : [ 35, 98, 117, 110, 100, 108, 101, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 28, 47, 97, 99, 116, 105, 118, 101, 47, 105, 112, 0, 0, 44, 115, 0, 0, 108, 111, 99, 97, 108, 104, 111, 115, 116, 0, 0, 0, 0, 0, 0, 24, 47, 97, 99, 116, 105, 118, 101, 47, 112, 111, 114, 116, 0, 0, 0, 0, 44, 105, 0, 0, 0, 0, 21, -77 ],
					"saved_bundle_length" : 76,
					"text" : "/active/ip : \"localhost\",\n/active/port : 5555",
					"textcolor" : [ 0.188, 0.188, 0.188, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-3",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 233.0, 436.0, 140.0, 22.0 ],
					"style" : "",
					"text" : "udpsend 127.0.0.1 4444"
				}

			}
, 			{
				"box" : 				{
					"fontface" : 0,
					"fontsize" : 12.0,
					"id" : "obj-2",
					"linecount" : 7,
					"maxclass" : "o.compose",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 53.0, 103.0, 448.0, 106.0 ],
					"saved_bundle_data" : [ 35, 98, 117, 110, 100, 108, 101, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -96, 47, 100, 111, 99, 0, 0, 0, 0, 44, 115, 0, 0, 116, 111, 32, 97, 99, 116, 105, 118, 97, 116, 101, 32, 100, 97, 116, 97, 32, 115, 117, 98, 115, 99, 114, 105, 112, 116, 105, 111, 110, 44, 32, 115, 101, 110, 100, 32, 47, 97, 99, 116, 105, 118, 101, 47, 105, 112, 32, 97, 110, 100, 32, 47, 97, 99, 116, 105, 118, 101, 47, 112, 111, 114, 116, 32, 118, 97, 108, 117, 101, 115, 32, 111, 110, 99, 101, 32, 101, 118, 101, 114, 121, 32, 53, 32, 115, 101, 99, 111, 110, 100, 115, 44, 32, 105, 110, 32, 116, 104, 101, 32, 102, 117, 116, 117, 114, 101, 32, 116, 104, 101, 114, 101, 32, 119, 105, 108, 108, 32, 98, 101, 32, 111, 116, 104, 101, 114, 32, 112, 97, 114, 97, 109, 101, 116, 101, 114, 115, 32, 97, 115, 32, 97, 119, 101, 108, 108, 0, 0, 0, 0, 0, 32, 47, 97, 99, 116, 105, 118, 101, 47, 105, 112, 0, 0, 44, 115, 0, 0, 121, 111, 117, 114, 32, 105, 112, 32, 97, 100, 100, 114, 101, 115, 115, 0, 0, 0, 0, 32, 47, 97, 99, 116, 105, 118, 101, 47, 112, 111, 114, 116, 0, 0, 0, 0, 44, 115, 0, 0, 112, 111, 114, 116, 32, 110, 117, 109, 98, 101, 114, 0, 0, 0, 0, 36, 47, 115, 101, 110, 100, 116, 111, 47, 115, 101, 114, 118, 101, 114, 47, 105, 112, 0, 0, 0, 44, 115, 0, 0, 49, 50, 55, 46, 48, 46, 48, 46, 49, 0, 0, 0, 0, 0, 0, 28, 47, 115, 101, 110, 100, 116, 111, 47, 115, 101, 114, 118, 101, 114, 47, 112, 111, 114, 116, 0, 44, 105, 0, 0, 0, 0, 17, 92 ],
					"saved_bundle_length" : 324,
					"text" : "/doc : \"to activate data subscription, send /active/ip and /active/port values once every 5 seconds, in the future there will be other parameters as awell\",\n/active/ip : \"your ip address\",\n/active/port : \"port number\",\n/sendto/server/ip : \"127.0.0.1\",\n/sendto/server/port : 4444",
					"textcolor" : [ 0.188, 0.188, 0.188, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-1",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 482.0, 17.0, 135.0, 22.0 ],
					"style" : "",
					"text" : "udpreceive 5555 cnmat"
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"destination" : [ "obj-6", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-1", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-8", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-10", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-3", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-5", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-13", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-6", 2 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-2", 1 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-6", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-7", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-6", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-5", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-8", 0 ]
				}

			}
 ],
		"dependency_cache" : [ 			{
				"name" : "o.compose.mxo",
				"type" : "iLaX"
			}
, 			{
				"name" : "o.route.mxo",
				"type" : "iLaX"
			}
, 			{
				"name" : "o.display.mxo",
				"type" : "iLaX"
			}
 ],
		"embedsnapshot" : 0,
		"bgfillcolor_type" : "gradient",
		"bgfillcolor_color1" : [ 0.376471, 0.384314, 0.4, 1.0 ],
		"bgfillcolor_color2" : [ 0.290196, 0.309804, 0.301961, 1.0 ],
		"bgfillcolor_color" : [ 0.290196, 0.309804, 0.301961, 1.0 ],
		"bgfillcolor_angle" : 270.0,
		"bgfillcolor_proportion" : 0.39
	}

}
