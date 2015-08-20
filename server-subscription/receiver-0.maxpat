{
	"patcher" : 	{
		"fileversion" : 1,
		"appversion" : 		{
			"major" : 7,
			"minor" : 0,
			"revision" : 5,
			"architecture" : "x64",
			"modernui" : 1
		}
,
		"rect" : [ 126.0, 45.0, 1206.0, 851.0 ],
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
					"id" : "obj-2",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "jit_matrix", "" ],
					"patching_rect" : [ 234.0, 275.0, 100.0, 22.0 ],
					"style" : "",
					"text" : "jit.resamp"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-16",
					"maxclass" : "button",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "bang" ],
					"patching_rect" : [ 259.0, 134.5, 24.0, 24.0 ],
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-9",
					"maxclass" : "button",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "bang" ],
					"patching_rect" : [ 312.0, 54.5, 24.0, 24.0 ],
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-18",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 259.0, 20.0, 72.0, 22.0 ],
					"style" : "",
					"text" : "loadmess 1"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-7",
					"maxclass" : "toggle",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "int" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 259.0, 54.5, 24.0, 24.0 ],
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"fontface" : 0,
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"id" : "obj-12",
					"maxclass" : "jit.fpsgui",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"patching_rect" : [ 216.0, 431.5, 80.0, 35.0 ],
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"fontface" : 0,
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"id" : "obj-14",
					"maxclass" : "jit.fpsgui",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"patching_rect" : [ 116.0, 431.5, 80.0, 35.0 ],
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-10",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patcher" : 					{
						"fileversion" : 1,
						"appversion" : 						{
							"major" : 7,
							"minor" : 0,
							"revision" : 5,
							"architecture" : "x64",
							"modernui" : 1
						}
,
						"rect" : [ -1920.0, 22.0, 1920.0, 1058.0 ],
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
						"toolbars_unpinned_last_save" : 0,
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
						"boxes" : [ 							{
								"box" : 								{
									"id" : "obj-30",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 3,
									"outlettype" : [ "", "", "FullPacket" ],
									"patching_rect" : [ 523.999939, 78.399986, 141.0, 22.0 ],
									"style" : "",
									"text" : "o.select /kinect1 /kinect2"
								}

							}
, 							{
								"box" : 								{
									"fontface" : 0,
									"fontsize" : 12.0,
									"id" : "obj-22",
									"linecount" : 10,
									"maxclass" : "o.compose",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 502.600006, 467.800018, 844.0, 146.0 ],
									"saved_bundle_data" : [ 35, 98, 117, 110, 100, 108, 101, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 84, 47, 107, 105, 110, 101, 99, 116, 50, 47, 115, 101, 116, 116, 105, 110, 103, 115, 47, 122, 95, 115, 108, 105, 99, 101, 47, 109, 105, 110, 0, 0, 0, 44, 115, 0, 0, 109, 105, 110, 32, 100, 105, 115, 116, 97, 110, 99, 101, 32, 102, 111, 114, 32, 112, 111, 105, 110, 116, 32, 99, 108, 111, 117, 100, 32, 40, 100, 101, 102, 97, 117, 108, 116, 32, 48, 46, 41, 0, 0, 0, -40, 119, 88, 10, 0, 0, 0, 84, 47, 107, 105, 110, 101, 99, 116, 50, 47, 115, 101, 116, 116, 105, 110, 103, 115, 47, 122, 95, 115, 108, 105, 99, 101, 47, 109, 97, 120, 0, 0, 0, 44, 115, 0, 0, 109, 97, 120, 32, 100, 105, 115, 116, 97, 110, 99, 101, 32, 102, 111, 114, 32, 112, 111, 105, 110, 116, 32, 99, 108, 111, 117, 100, 32, 40, 100, 101, 102, 97, 117, 108, 116, 32, 49, 46, 41, 0, 0, 0, 67, -66, -107, 119, 0, 0, 0, 112, 47, 107, 105, 110, 101, 99, 116, 50, 47, 115, 101, 116, 116, 105, 110, 103, 115, 47, 112, 111, 105, 110, 116, 95, 115, 99, 97, 108, 97, 114, 0, 0, 44, 115, 0, 0, 115, 99, 97, 108, 97, 114, 32, 116, 111, 32, 114, 101, 100, 117, 99, 101, 32, 110, 117, 109, 98, 101, 114, 32, 111, 102, 32, 112, 111, 105, 110, 116, 115, 32, 105, 110, 32, 112, 111, 105, 110, 116, 32, 99, 108, 111, 117, 100, 44, 32, 48, 45, 49, 44, 32, 40, 100, 101, 102, 97, 117, 108, 116, 32, 48, 46, 50, 41, 0, 0, 0, 0, 0, 74, 55, 39, 0, 0, 0, 48, 47, 107, 105, 110, 101, 99, 116, 50, 47, 115, 101, 110, 100, 116, 111, 47, 115, 101, 114, 118, 101, 114, 47, 105, 112, 0, 0, 0, 44, 115, 0, 0, 49, 50, 56, 46, 51, 50, 46, 49, 50, 50, 46, 49, 50, 55, 0, 0, 0, 0, 0, 36, 47, 107, 105, 110, 101, 99, 116, 50, 47, 115, 101, 110, 100, 116, 111, 47, 115, 101, 114, 118, 101, 114, 47, 112, 111, 114, 116, 0, 44, 105, 0, 0, 0, 0, 11, -70, 0, 0, 0, 116, 47, 107, 105, 110, 101, 99, 116, 50, 47, 100, 111, 99, 0, 0, 0, 0, 44, 115, 0, 0, 116, 111, 32, 97, 99, 116, 105, 118, 97, 116, 101, 32, 100, 97, 116, 97, 32, 115, 117, 98, 115, 99, 114, 105, 112, 116, 105, 111, 110, 44, 32, 115, 101, 110, 100, 32, 47, 97, 99, 116, 105, 118, 101, 47, 105, 112, 32, 118, 97, 108, 117, 101, 32, 111, 110, 99, 101, 32, 101, 118, 101, 114, 121, 32, 53, 32, 115, 101, 99, 111, 110, 100, 115, 44, 32, 119, 105, 116, 104, 32, 115, 116, 114, 101, 97, 109, 32, 114, 101, 113, 117, 101, 115, 116, 115, 0, 0, 0, 0, 48, 47, 107, 105, 110, 101, 99, 116, 50, 47, 115, 101, 114, 118, 101, 114, 47, 97, 99, 116, 105, 118, 101, 47, 105, 112, 0, 0, 0, 44, 115, 0, 0, 121, 111, 117, 114, 32, 105, 112, 32, 97, 100, 100, 114, 101, 115, 115, 0, 0, 0, 0, 44, 47, 107, 105, 110, 101, 99, 116, 50, 47, 115, 101, 114, 118, 101, 114, 47, 97, 99, 116, 105, 118, 101, 47, 112, 111, 114, 116, 0, 44, 115, 0, 0, 112, 111, 114, 116, 32, 110, 117, 109, 98, 101, 114, 0, 0, 0, 0, 48, 47, 107, 105, 110, 101, 99, 116, 50, 47, 115, 101, 114, 118, 101, 114, 47, 97, 99, 116, 105, 118, 101, 47, 115, 107, 101, 108, 101, 116, 111, 110, 0, 44, 115, 0, 0, 116, 114, 117, 101, 47, 102, 97, 108, 115, 101, 0, 0, 0, 0, 0, 48, 47, 107, 105, 110, 101, 99, 116, 50, 47, 115, 101, 114, 118, 101, 114, 47, 97, 99, 116, 105, 118, 101, 47, 106, 105, 116, 116, 101, 114, 0, 0, 0, 44, 115, 0, 0, 116, 114, 117, 101, 47, 102, 97, 108, 115, 101, 0, 0 ],
									"saved_bundle_length" : 724,
									"text" : "/kinect2/settings/z_slice/min : \"min distance for point cloud (default 0.)\",\n/kinect2/settings/z_slice/max : \"max distance for point cloud (default 1.)\",\n/kinect2/settings/point_scalar : \"scalar to reduce number of points in point cloud, 0-1, (default 0.2)\",\n/kinect2/sendto/server/ip : \"128.32.122.127\",\n/kinect2/sendto/server/port : 3002,\n/kinect2/doc : \"to activate data subscription, send /active/ip value once every 5 seconds, with stream requests\",\n/kinect2/server/active/ip : \"your ip address\",\n/kinect2/server/active/port : \"port number\",\n/kinect2/server/active/skeleton : \"true/false\",\n/kinect2/server/active/jitter : \"true/false\"",
									"textcolor" : [ 0.188, 0.188, 0.188, 1.0 ]
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-28",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "", "" ],
									"patching_rect" : [ 202.399994, 95.900002, 100.0, 22.0 ],
									"style" : "",
									"text" : "t l l"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-27",
									"maxclass" : "newobj",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "FullPacket" ],
									"patching_rect" : [ 202.399994, 65.900002, 100.0, 22.0 ],
									"style" : "",
									"text" : "o.union"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-19",
									"items" : [ "fe80:0:0:0:6a5b:35ff:fe98:9038%10", ",", "128.32.122.223" ],
									"maxclass" : "umenu",
									"numinlets" : 1,
									"numoutlets" : 3,
									"outlettype" : [ "int", "", "" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 346.800018, 727.0, 151.0, 22.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-17",
									"maxclass" : "button",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "bang" ],
									"patching_rect" : [ 346.800018, 611.0, 24.0, 24.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-4",
									"items" : [ "en4", ",", "lo0" ],
									"maxclass" : "umenu",
									"numinlets" : 1,
									"numoutlets" : 3,
									"outlettype" : [ "int", "", "" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 404.800018, 691.0, 100.0, 22.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-6",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "", "" ],
									"patching_rect" : [ 346.800018, 654.0, 77.0, 22.0 ],
									"style" : "",
									"text" : "mxj net.local"
								}

							}
, 							{
								"box" : 								{
									"comment" : "",
									"id" : "obj-1",
									"maxclass" : "inlet",
									"numinlets" : 0,
									"numoutlets" : 1,
									"outlettype" : [ "int" ],
									"patching_rect" : [ 48.400002, 316.399994, 30.0, 30.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-8",
									"maxclass" : "newobj",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "bang" ],
									"patching_rect" : [ 48.400002, 433.399994, 71.0, 22.0 ],
									"style" : "",
									"text" : "metro 5000"
								}

							}
, 							{
								"box" : 								{
									"fontface" : 0,
									"fontsize" : 12.0,
									"id" : "obj-5",
									"linecount" : 5,
									"maxclass" : "o.compose",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 48.400002, 471.399994, 355.0, 78.0 ],
									"saved_bundle_data" : [ 35, 98, 117, 110, 100, 108, 101, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 40, 47, 42, 47, 115, 101, 114, 118, 101, 114, 47, 97, 99, 116, 105, 118, 101, 47, 105, 112, 0, 44, 115, 0, 0, 49, 50, 56, 46, 51, 50, 46, 49, 50, 50, 46, 50, 50, 51, 0, 0, 0, 0, 0, 36, 47, 107, 105, 110, 101, 99, 116, 49, 47, 115, 101, 114, 118, 101, 114, 47, 97, 99, 116, 105, 118, 101, 47, 106, 105, 116, 116, 101, 114, 0, 0, 0, 44, 84, 0, 0, 0, 0, 0, 36, 47, 107, 105, 110, 101, 99, 116, 49, 47, 115, 101, 114, 118, 101, 114, 47, 97, 99, 116, 105, 118, 101, 47, 115, 107, 101, 108, 101, 116, 111, 110, 0, 44, 84, 0, 0, 0, 0, 0, 36, 47, 107, 105, 110, 101, 99, 116, 50, 47, 115, 101, 114, 118, 101, 114, 47, 97, 99, 116, 105, 118, 101, 47, 106, 105, 116, 116, 101, 114, 0, 0, 0, 44, 84, 0, 0, 0, 0, 0, 36, 47, 107, 105, 110, 101, 99, 116, 50, 47, 115, 101, 114, 118, 101, 114, 47, 97, 99, 116, 105, 118, 101, 47, 115, 107, 101, 108, 101, 116, 111, 110, 0, 44, 84, 0, 0 ],
									"saved_bundle_length" : 220,
									"text" : "/*/server/active/ip : \"128.32.122.223\",\n/kinect1/server/active/jitter : true,\n/kinect1/server/active/skeleton : true,\n/kinect2/server/active/jitter : true,\n/kinect2/server/active/skeleton : true",
									"textcolor" : [ 0.188, 0.188, 0.188, 1.0 ]
								}

							}
, 							{
								"box" : 								{
									"comment" : "",
									"id" : "obj-7",
									"maxclass" : "inlet",
									"numinlets" : 0,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 202.399994, 21.4, 30.0, 30.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"comment" : "",
									"id" : "obj-9",
									"maxclass" : "outlet",
									"numinlets" : 1,
									"numoutlets" : 0,
									"patching_rect" : [ 48.400002, 594.400024, 30.0, 30.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"fontface" : 0,
									"fontsize" : 12.0,
									"id" : "obj-2",
									"linecount" : 22,
									"maxclass" : "o.compose",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 206.399994, 142.899994, 786.0, 309.0 ],
									"saved_bundle_data" : [ 35, 98, 117, 110, 100, 108, 101, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 56, 47, 107, 105, 110, 101, 99, 116, 49, 47, 115, 101, 110, 100, 116, 111, 47, 115, 101, 114, 118, 101, 114, 47, 105, 112, 0, 0, 0, 44, 115, 0, 0, 105, 112, 32, 97, 100, 100, 114, 101, 115, 115, 32, 115, 101, 116, 32, 111, 110, 32, 108, 111, 97, 100, 0, 0, 0, 0, 0, 36, 47, 107, 105, 110, 101, 99, 116, 49, 47, 115, 101, 110, 100, 116, 111, 47, 115, 101, 114, 118, 101, 114, 47, 112, 111, 114, 116, 0, 44, 105, 0, 0, 0, 0, 11, -72, 0, 0, 0, 116, 47, 107, 105, 110, 101, 99, 116, 49, 47, 100, 111, 99, 0, 0, 0, 0, 44, 115, 0, 0, 116, 111, 32, 97, 99, 116, 105, 118, 97, 116, 101, 32, 100, 97, 116, 97, 32, 115, 117, 98, 115, 99, 114, 105, 112, 116, 105, 111, 110, 44, 32, 115, 101, 110, 100, 32, 47, 97, 99, 116, 105, 118, 101, 47, 105, 112, 32, 118, 97, 108, 117, 101, 32, 111, 110, 99, 101, 32, 101, 118, 101, 114, 121, 32, 53, 32, 115, 101, 99, 111, 110, 100, 115, 44, 32, 119, 105, 116, 104, 32, 115, 116, 114, 101, 97, 109, 32, 114, 101, 113, 117, 101, 115, 116, 115, 0, 0, 0, 0, 48, 47, 107, 105, 110, 101, 99, 116, 49, 47, 115, 101, 114, 118, 101, 114, 47, 97, 99, 116, 105, 118, 101, 47, 105, 112, 0, 0, 0, 44, 115, 0, 0, 121, 111, 117, 114, 32, 105, 112, 32, 97, 100, 100, 114, 101, 115, 115, 0, 0, 0, 0, 44, 47, 107, 105, 110, 101, 99, 116, 49, 47, 115, 101, 114, 118, 101, 114, 47, 97, 99, 116, 105, 118, 101, 47, 112, 111, 114, 116, 0, 44, 115, 0, 0, 112, 111, 114, 116, 32, 110, 117, 109, 98, 101, 114, 0, 0, 0, 0, 48, 47, 107, 105, 110, 101, 99, 116, 49, 47, 115, 101, 114, 118, 101, 114, 47, 97, 99, 116, 105, 118, 101, 47, 115, 107, 101, 108, 101, 116, 111, 110, 0, 44, 115, 0, 0, 116, 114, 117, 101, 47, 102, 97, 108, 115, 101, 0, 0, 0, 0, 0, 48, 47, 107, 105, 110, 101, 99, 116, 49, 47, 115, 101, 114, 118, 101, 114, 47, 97, 99, 116, 105, 118, 101, 47, 106, 105, 116, 116, 101, 114, 0, 0, 0, 44, 115, 0, 0, 116, 114, 117, 101, 47, 102, 97, 108, 115, 101, 0, 0, 0, 0, 0, 84, 47, 107, 105, 110, 101, 99, 116, 49, 47, 115, 101, 116, 116, 105, 110, 103, 115, 47, 122, 95, 115, 108, 105, 99, 101, 47, 109, 105, 110, 0, 0, 0, 44, 115, 0, 0, 109, 105, 110, 32, 100, 105, 115, 116, 97, 110, 99, 101, 32, 102, 111, 114, 32, 112, 111, 105, 110, 116, 32, 99, 108, 111, 117, 100, 32, 40, 100, 101, 102, 97, 117, 108, 116, 32, 48, 46, 41, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 84, 47, 107, 105, 110, 101, 99, 116, 49, 47, 115, 101, 116, 116, 105, 110, 103, 115, 47, 122, 95, 115, 108, 105, 99, 101, 47, 109, 97, 120, 0, 0, 0, 44, 115, 0, 0, 109, 97, 120, 32, 100, 105, 115, 116, 97, 110, 99, 101, 32, 102, 111, 114, 32, 112, 111, 105, 110, 116, 32, 99, 108, 111, 117, 100, 32, 40, 100, 101, 102, 97, 117, 108, 116, 32, 49, 46, 41, 0, 0, 0, 0, 66, -127, 32, 0, 0, 0, 112, 47, 107, 105, 110, 101, 99, 116, 49, 47, 115, 101, 116, 116, 105, 110, 103, 115, 47, 112, 111, 105, 110, 116, 95, 115, 99, 97, 108, 97, 114, 0, 0, 44, 115, 0, 0, 115, 99, 97, 108, 97, 114, 32, 116, 111, 32, 114, 101, 100, 117, 99, 101, 32, 110, 117, 109, 98, 101, 114, 32, 111, 102, 32, 112, 111, 105, 110, 116, 115, 32, 105, 110, 32, 112, 111, 105, 110, 116, 32, 99, 108, 111, 117, 100, 44, 32, 48, 45, 49, 44, 32, 40, 100, 101, 102, 97, 117, 108, 116, 32, 48, 46, 50, 41, 0, 0, 0, 0, -51, 26, 100, 105, 0, 0, 0, 84, 47, 107, 105, 110, 101, 99, 116, 50, 47, 115, 101, 116, 116, 105, 110, 103, 115, 47, 122, 95, 115, 108, 105, 99, 101, 47, 109, 105, 110, 0, 0, 0, 44, 115, 0, 0, 109, 105, 110, 32, 100, 105, 115, 116, 97, 110, 99, 101, 32, 102, 111, 114, 32, 112, 111, 105, 110, 116, 32, 99, 108, 111, 117, 100, 32, 40, 100, 101, 102, 97, 117, 108, 116, 32, 48, 46, 41, 0, 0, 0, -40, 119, 88, 10, 0, 0, 0, 84, 47, 107, 105, 110, 101, 99, 116, 50, 47, 115, 101, 116, 116, 105, 110, 103, 115, 47, 122, 95, 115, 108, 105, 99, 101, 47, 109, 97, 120, 0, 0, 0, 44, 115, 0, 0, 109, 97, 120, 32, 100, 105, 115, 116, 97, 110, 99, 101, 32, 102, 111, 114, 32, 112, 111, 105, 110, 116, 32, 99, 108, 111, 117, 100, 32, 40, 100, 101, 102, 97, 117, 108, 116, 32, 49, 46, 41, 0, 0, 0, 67, -66, -107, 119, 0, 0, 0, 112, 47, 107, 105, 110, 101, 99, 116, 50, 47, 115, 101, 116, 116, 105, 110, 103, 115, 47, 112, 111, 105, 110, 116, 95, 115, 99, 97, 108, 97, 114, 0, 0, 44, 115, 0, 0, 115, 99, 97, 108, 97, 114, 32, 116, 111, 32, 114, 101, 100, 117, 99, 101, 32, 110, 117, 109, 98, 101, 114, 32, 111, 102, 32, 112, 111, 105, 110, 116, 115, 32, 105, 110, 32, 112, 111, 105, 110, 116, 32, 99, 108, 111, 117, 100, 44, 32, 48, 45, 49, 44, 32, 40, 100, 101, 102, 97, 117, 108, 116, 32, 48, 46, 50, 41, 0, 0, 0, 0, 0, 74, 55, 39, 0, 0, 0, 48, 47, 107, 105, 110, 101, 99, 116, 50, 47, 115, 101, 110, 100, 116, 111, 47, 115, 101, 114, 118, 101, 114, 47, 105, 112, 0, 0, 0, 44, 115, 0, 0, 49, 50, 56, 46, 51, 50, 46, 49, 50, 50, 46, 49, 50, 55, 0, 0, 0, 0, 0, 36, 47, 107, 105, 110, 101, 99, 116, 50, 47, 115, 101, 110, 100, 116, 111, 47, 115, 101, 114, 118, 101, 114, 47, 112, 111, 114, 116, 0, 44, 105, 0, 0, 0, 0, 11, -70, 0, 0, 0, 116, 47, 107, 105, 110, 101, 99, 116, 50, 47, 100, 111, 99, 0, 0, 0, 0, 44, 115, 0, 0, 116, 111, 32, 97, 99, 116, 105, 118, 97, 116, 101, 32, 100, 97, 116, 97, 32, 115, 117, 98, 115, 99, 114, 105, 112, 116, 105, 111, 110, 44, 32, 115, 101, 110, 100, 32, 47, 97, 99, 116, 105, 118, 101, 47, 105, 112, 32, 118, 97, 108, 117, 101, 32, 111, 110, 99, 101, 32, 101, 118, 101, 114, 121, 32, 53, 32, 115, 101, 99, 111, 110, 100, 115, 44, 32, 119, 105, 116, 104, 32, 115, 116, 114, 101, 97, 109, 32, 114, 101, 113, 117, 101, 115, 116, 115, 0, 0, 0, 0, 48, 47, 107, 105, 110, 101, 99, 116, 50, 47, 115, 101, 114, 118, 101, 114, 47, 97, 99, 116, 105, 118, 101, 47, 105, 112, 0, 0, 0, 44, 115, 0, 0, 121, 111, 117, 114, 32, 105, 112, 32, 97, 100, 100, 114, 101, 115, 115, 0, 0, 0, 0, 44, 47, 107, 105, 110, 101, 99, 116, 50, 47, 115, 101, 114, 118, 101, 114, 47, 97, 99, 116, 105, 118, 101, 47, 112, 111, 114, 116, 0, 44, 115, 0, 0, 112, 111, 114, 116, 32, 110, 117, 109, 98, 101, 114, 0, 0, 0, 0, 48, 47, 107, 105, 110, 101, 99, 116, 50, 47, 115, 101, 114, 118, 101, 114, 47, 97, 99, 116, 105, 118, 101, 47, 115, 107, 101, 108, 101, 116, 111, 110, 0, 44, 115, 0, 0, 116, 114, 117, 101, 47, 102, 97, 108, 115, 101, 0, 0, 0, 0, 0, 48, 47, 107, 105, 110, 101, 99, 116, 50, 47, 115, 101, 114, 118, 101, 114, 47, 97, 99, 116, 105, 118, 101, 47, 106, 105, 116, 116, 101, 114, 0, 0, 0, 44, 115, 0, 0, 116, 114, 117, 101, 47, 102, 97, 108, 115, 101, 0, 0 ],
									"saved_bundle_length" : 1440,
									"text" : "/kinect1/sendto/server/ip : \"ip address set on load\",\n/kinect1/sendto/server/port : 3000,\n/kinect1/doc : \"to activate data subscription, send /active/ip value once every 5 seconds, with stream requests\",\n/kinect1/server/active/ip : \"your ip address\",\n/kinect1/server/active/port : \"port number\",\n/kinect1/server/active/skeleton : \"true/false\",\n/kinect1/server/active/jitter : \"true/false\",\n/kinect1/settings/z_slice/min : \"min distance for point cloud (default 0.)\",\n/kinect1/settings/z_slice/max : \"max distance for point cloud (default 1.)\",\n/kinect1/settings/point_scalar : \"scalar to reduce number of points in point cloud, 0-1, (default 0.2)\",\n/kinect2/settings/z_slice/min : \"min distance for point cloud (default 0.)\",\n/kinect2/settings/z_slice/max : \"max distance for point cloud (default 1.)\",\n/kinect2/settings/point_scalar : \"scalar to reduce number of points in point cloud, 0-1, (default 0.2)\",\n/kinect2/sendto/server/ip : \"128.32.122.127\",\n/kinect2/sendto/server/port : 3002,\n/kinect2/doc : \"to activate data subscription, send /active/ip value once every 5 seconds, with stream requests\",\n/kinect2/server/active/ip : \"your ip address\",\n/kinect2/server/active/port : \"port number\",\n/kinect2/server/active/skeleton : \"true/false\",\n/kinect2/server/active/jitter : \"true/false\"",
									"textcolor" : [ 0.188, 0.188, 0.188, 1.0 ]
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"destination" : [ "obj-8", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-1", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-6", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-17", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-28", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-27", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-2", 1 ],
									"disabled" : 0,
									"hidden" : 0,
									"midpoints" : [ 211.899994, 129.650002, 982.899994, 129.650002 ],
									"source" : [ "obj-28", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-27", 1 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-28", 1 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-30", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-28", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-22", 1 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-30", 1 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-6", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"midpoints" : [ 454.800018, 722.999977, 519.549988, 722.999977, 519.549988, 642.999977, 356.300018, 642.999977 ],
									"source" : [ "obj-4", 1 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-9", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-5", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-19", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-6", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-4", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-6", 1 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-27", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-7", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-5", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-8", 0 ]
								}

							}
 ]
					}
,
					"patching_rect" : [ 259.0, 92.0, 112.0, 22.0 ],
					"saved_object_attributes" : 					{
						"description" : "",
						"digest" : "",
						"globalpatchername" : "",
						"style" : "",
						"tags" : ""
					}
,
					"style" : "",
					"text" : "p sever-connection"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-3",
					"maxclass" : "button",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "bang" ],
					"patching_rect" : [ 393.0, 100.0, 24.0, 24.0 ],
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"format" : 6,
					"id" : "obj-61",
					"maxclass" : "flonum",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 705.0, 174.5, 50.0, 22.0 ],
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"format" : 6,
					"id" : "obj-60",
					"maxclass" : "flonum",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 1065.0, 192.0, 50.0, 22.0 ],
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-57",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "FullPacket" ],
					"patching_rect" : [ 538.5, 299.0, 157.0, 22.0 ],
					"style" : "",
					"text" : "o.prepend /kinect1/settings"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-56",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "FullPacket" ],
					"patching_rect" : [ 819.5, 291.5, 157.0, 22.0 ],
					"style" : "",
					"text" : "o.prepend /kinect2/settings"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-52",
					"maxclass" : "button",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "bang" ],
					"patching_rect" : [ 534.0, 204.5, 24.0, 24.0 ],
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"format" : 6,
					"id" : "obj-53",
					"maxclass" : "flonum",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 596.5, 169.0, 50.0, 22.0 ],
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"format" : 6,
					"id" : "obj-54",
					"maxclass" : "flonum",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 482.0, 180.5, 50.0, 22.0 ],
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-55",
					"maxclass" : "newobj",
					"numinlets" : 3,
					"numoutlets" : 1,
					"outlettype" : [ "FullPacket" ],
					"patching_rect" : [ 482.0, 234.0, 306.0, 22.0 ],
					"style" : "",
					"text" : "o.pack /z_slice/min 0. /z_slice/max 1. /point_scalar 0.02"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-51",
					"maxclass" : "button",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "bang" ],
					"patching_rect" : [ 868.0, 209.0, 24.0, 24.0 ],
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"format" : 6,
					"id" : "obj-49",
					"maxclass" : "flonum",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 937.0, 192.0, 50.0, 22.0 ],
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"format" : 6,
					"id" : "obj-47",
					"maxclass" : "flonum",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 831.0, 185.0, 50.0, 22.0 ],
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-48",
					"maxclass" : "newobj",
					"numinlets" : 3,
					"numoutlets" : 1,
					"outlettype" : [ "FullPacket" ],
					"patching_rect" : [ 831.0, 239.0, 299.0, 22.0 ],
					"style" : "",
					"text" : "o.pack /z_slice/min 0. /z_slice/max 1. /point_scalar 0.2"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-42",
					"maxclass" : "button",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "bang" ],
					"patching_rect" : [ 303.0, 399.0, 24.0, 24.0 ],
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-40",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 1247.0, 434.0, 50.0, 22.0 ],
					"style" : "",
					"text" : "clear"
				}

			}
, 			{
				"box" : 				{
					"fontface" : 0,
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"id" : "obj-38",
					"maxclass" : "jit.fpsgui",
					"mode" : 3,
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"patching_rect" : [ 216.0, 389.0, 80.0, 35.0 ],
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-37",
					"linecount" : 2,
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 3,
					"outlettype" : [ "jit_matrix", "", "" ],
					"patching_rect" : [ 199.5, 341.0, 78.0, 35.0 ],
					"style" : "",
					"text" : "jit.net.recv @port 4002"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-36",
					"maxclass" : "button",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "bang" ],
					"patching_rect" : [ 68.0, 400.0, 24.0, 24.0 ],
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-34",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 599.5, 440.0, 173.0, 22.0 ],
					"style" : "",
					"text" : "udpsend 128.32.122.127 3002"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-33",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 3,
					"outlettype" : [ "", "", "FullPacket" ],
					"patching_rect" : [ 538.5, 399.0, 141.0, 22.0 ],
					"style" : "",
					"text" : "o.route /kinect1 /kinect2"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-31",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 538.5, 478.0, 173.0, 22.0 ],
					"style" : "",
					"text" : "udpsend 128.32.122.225 3001"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-30",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 3,
					"outlettype" : [ "", "", "FullPacket" ],
					"patching_rect" : [ 1061.0, 522.0, 141.0, 22.0 ],
					"style" : "",
					"text" : "o.select /kinect1 /kinect2"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-28",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"patching_rect" : [ 1061.0, 482.0, 100.0, 22.0 ],
					"style" : "",
					"text" : "t l l"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-27",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "FullPacket" ],
					"patching_rect" : [ 1061.0, 452.0, 100.0, 22.0 ],
					"style" : "",
					"text" : "o.union"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-26",
					"linecount" : 7,
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 1319.0, 413.0, 171.0, 100.0 ],
					"style" : "",
					"text" : "client UDP port 4000\n\nkinect1 server UDP port 3001\nkinect1 client jitter port 4001\n\nkinect2 server UDP port 3002\nkinect2 client UDP port 4002\n"
				}

			}
, 			{
				"box" : 				{
					"fontface" : 0,
					"fontsize" : 12.0,
					"id" : "obj-22",
					"linecount" : 11,
					"maxclass" : "o.compose",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 550.0, 585.0, 844.0, 160.0 ],
					"saved_bundle_data" : [ 35, 98, 117, 110, 100, 108, 101, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 56, 47, 107, 105, 110, 101, 99, 116, 50, 47, 115, 101, 110, 100, 116, 111, 47, 115, 101, 114, 118, 101, 114, 47, 105, 112, 0, 0, 0, 44, 115, 0, 0, 105, 112, 32, 97, 100, 100, 114, 101, 115, 115, 32, 115, 101, 116, 32, 111, 110, 32, 108, 111, 97, 100, 0, 0, 0, 0, 0, 36, 47, 107, 105, 110, 101, 99, 116, 50, 47, 115, 101, 110, 100, 116, 111, 47, 115, 101, 114, 118, 101, 114, 47, 112, 111, 114, 116, 0, 44, 105, 0, 0, 0, 0, 11, -70, 0, 0, 0, 116, 47, 107, 105, 110, 101, 99, 116, 50, 47, 100, 111, 99, 0, 0, 0, 0, 44, 115, 0, 0, 116, 111, 32, 97, 99, 116, 105, 118, 97, 116, 101, 32, 100, 97, 116, 97, 32, 115, 117, 98, 115, 99, 114, 105, 112, 116, 105, 111, 110, 44, 32, 115, 101, 110, 100, 32, 47, 97, 99, 116, 105, 118, 101, 47, 105, 112, 32, 118, 97, 108, 117, 101, 32, 111, 110, 99, 101, 32, 101, 118, 101, 114, 121, 32, 53, 32, 115, 101, 99, 111, 110, 100, 115, 44, 32, 119, 105, 116, 104, 32, 115, 116, 114, 101, 97, 109, 32, 114, 101, 113, 117, 101, 115, 116, 115, 0, 0, 0, 0, 48, 47, 107, 105, 110, 101, 99, 116, 50, 47, 115, 101, 114, 118, 101, 114, 47, 97, 99, 116, 105, 118, 101, 47, 105, 112, 0, 0, 0, 44, 115, 0, 0, 121, 111, 117, 114, 32, 105, 112, 32, 97, 100, 100, 114, 101, 115, 115, 0, 0, 0, 0, 44, 47, 107, 105, 110, 101, 99, 116, 50, 47, 115, 101, 114, 118, 101, 114, 47, 97, 99, 116, 105, 118, 101, 47, 112, 111, 114, 116, 0, 44, 115, 0, 0, 112, 111, 114, 116, 32, 110, 117, 109, 98, 101, 114, 0, 0, 0, 0, 48, 47, 107, 105, 110, 101, 99, 116, 50, 47, 115, 101, 114, 118, 101, 114, 47, 97, 99, 116, 105, 118, 101, 47, 115, 107, 101, 108, 101, 116, 111, 110, 0, 44, 115, 0, 0, 116, 114, 117, 101, 47, 102, 97, 108, 115, 101, 0, 0, 0, 0, 0, 48, 47, 107, 105, 110, 101, 99, 116, 50, 47, 115, 101, 114, 118, 101, 114, 47, 97, 99, 116, 105, 118, 101, 47, 106, 105, 116, 116, 101, 114, 0, 0, 0, 44, 115, 0, 0, 116, 114, 117, 101, 47, 102, 97, 108, 115, 101, 0, 0, 0, 0, 0, 84, 47, 107, 105, 110, 101, 99, 116, 50, 47, 115, 101, 116, 116, 105, 110, 103, 115, 47, 122, 95, 115, 108, 105, 99, 101, 47, 109, 105, 110, 0, 0, 0, 44, 115, 0, 0, 109, 105, 110, 32, 100, 105, 115, 116, 97, 110, 99, 101, 32, 102, 111, 114, 32, 112, 111, 105, 110, 116, 32, 99, 108, 111, 117, 100, 32, 40, 100, 101, 102, 97, 117, 108, 116, 32, 48, 46, 41, 0, 0, 0, 96, -7, -93, 10, 0, 0, 0, 84, 47, 107, 105, 110, 101, 99, 116, 50, 47, 115, 101, 116, 116, 105, 110, 103, 115, 47, 122, 95, 115, 108, 105, 99, 101, 47, 109, 97, 120, 0, 0, 0, 44, 115, 0, 0, 109, 97, 120, 32, 100, 105, 115, 116, 97, 110, 99, 101, 32, 102, 111, 114, 32, 112, 111, 105, 110, 116, 32, 99, 108, 111, 117, 100, 32, 40, 100, 101, 102, 97, 117, 108, 116, 32, 49, 46, 41, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 112, 47, 107, 105, 110, 101, 99, 116, 50, 47, 115, 101, 116, 116, 105, 110, 103, 115, 47, 112, 111, 105, 110, 116, 95, 115, 99, 97, 108, 97, 114, 0, 0, 44, 115, 0, 0, 115, 99, 97, 108, 97, 114, 32, 116, 111, 32, 114, 101, 100, 117, 99, 101, 32, 110, 117, 109, 98, 101, 114, 32, 111, 102, 32, 112, 111, 105, 110, 116, 115, 32, 105, 110, 32, 112, 111, 105, 110, 116, 32, 99, 108, 111, 117, 100, 44, 32, 48, 45, 49, 44, 32, 40, 100, 101, 102, 97, 117, 108, 116, 32, 48, 46, 50, 41, 0, 0, 0, 0, 88, 13, 42, 52, 0, 0, 0, 40, 47, 107, 105, 110, 101, 99, 116, 50, 47, 115, 101, 110, 100, 116, 111, 47, 107, 105, 110, 101, 99, 116, 50, 47, 112, 111, 114, 116, 0, 0, 0, 0, 44, 105, 0, 0, 0, 0, 26, 10 ],
					"saved_bundle_length" : 776,
					"text" : "/kinect2/sendto/server/ip : \"ip address set on load\",\n/kinect2/sendto/server/port : 3002,\n/kinect2/doc : \"to activate data subscription, send /active/ip value once every 5 seconds, with stream requests\",\n/kinect2/server/active/ip : \"your ip address\",\n/kinect2/server/active/port : \"port number\",\n/kinect2/server/active/skeleton : \"true/false\",\n/kinect2/server/active/jitter : \"true/false\",\n/kinect2/settings/z_slice/min : \"min distance for point cloud (default 0.)\",\n/kinect2/settings/z_slice/max : \"max distance for point cloud (default 1.)\",\n/kinect2/settings/point_scalar : \"scalar to reduce number of points in point cloud, 0-1, (default 0.2)\",\n/kinect2/sendto/kinect2/port : 6666",
					"textcolor" : [ 0.188, 0.188, 0.188, 1.0 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-11",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 729.5, 400.0, 122.0, 20.0 ],
					"style" : "",
					"text" : "<< set server IP here"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-32",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 0,
					"patcher" : 					{
						"fileversion" : 1,
						"appversion" : 						{
							"major" : 7,
							"minor" : 0,
							"revision" : 5,
							"architecture" : "x64",
							"modernui" : 1
						}
,
						"rect" : [ 239.0, 45.0, 1197.0, 851.0 ],
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
						"boxes" : [ 							{
								"box" : 								{
									"attr" : "automatic",
									"id" : "obj-70",
									"maxclass" : "attrui",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 1046.5, 421.0, 150.0, 22.0 ],
									"presentation_rect" : [ 1046.5, 424.0, 0.0, 0.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"attr" : "draw_mode",
									"id" : "obj-69",
									"maxclass" : "attrui",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 487.0, 603.0, 150.0, 22.0 ],
									"presentation_rect" : [ 488.0, 603.0, 0.0, 0.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-65",
									"maxclass" : "newobj",
									"numinlets" : 3,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 639.0, 110.5, 100.0, 22.0 ],
									"style" : "",
									"text" : "clip 1 400"
								}

							}
, 							{
								"box" : 								{
									"attr" : "point_size",
									"id" : "obj-62",
									"maxclass" : "attrui",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 644.0, 564.0, 150.0, 22.0 ],
									"presentation_rect" : [ 648.0, 540.0, 0.0, 0.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"fontface" : 0,
									"fontname" : "Arial",
									"fontsize" : 12.0,
									"id" : "obj-61",
									"maxclass" : "jit.fpsgui",
									"mode" : 3,
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "", "" ],
									"patching_rect" : [ 778.0, 267.5, 80.0, 35.0 ],
									"presentation_rect" : [ 434.5, 182.5, 0.0, 0.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-60",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 639.0, 178.5, 57.0, 22.0 ],
									"presentation_rect" : [ 588.0, 182.5, 0.0, 0.0 ],
									"style" : "",
									"text" : "dim $1 1"
								}

							}
, 							{
								"box" : 								{
									"format" : 6,
									"id" : "obj-59",
									"maxclass" : "flonum",
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "", "bang" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 755.0, 104.0, 50.0, 22.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-57",
									"maxclass" : "newobj",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "float" ],
									"patching_rect" : [ 692.5, 151.0, 29.5, 22.0 ],
									"style" : "",
									"text" : "* 1."
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-56",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "jit_matrix", "" ],
									"patching_rect" : [ 639.0, 209.5, 180.0, 22.0 ],
									"style" : "",
									"text" : "jit.matrix 6 float32 1 1 @adapt 0"
								}

							}
, 							{
								"box" : 								{
									"attr" : "automatic",
									"id" : "obj-55",
									"maxclass" : "attrui",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 482.0, 414.0, 150.0, 22.0 ],
									"presentation_rect" : [ 475.0, 390.0, 0.0, 0.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"attr" : "automatic",
									"id" : "obj-53",
									"maxclass" : "attrui",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 644.0, 414.0, 150.0, 22.0 ],
									"presentation_rect" : [ 646.0, 393.0, 0.0, 0.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-52",
									"linecount" : 3,
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 3,
									"outlettype" : [ "jit_matrix", "jit_matrix", "" ],
									"patching_rect" : [ 639.0, 346.0, 83.0, 49.0 ],
									"presentation_rect" : [ 665.0, 241.0, 0.0, 0.0 ],
									"style" : "",
									"text" : "jit.unpack 2 @offset 0 3 @jump 3 3"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-51",
									"linecount" : 6,
									"maxclass" : "newobj",
									"numinlets" : 9,
									"numoutlets" : 2,
									"outlettype" : [ "jit_matrix", "" ],
									"patching_rect" : [ 639.0, 462.0, 125.0, 89.0 ],
									"style" : "",
									"text" : "jit.gl.mesh points @layer 0 @draw_mode points @point_size 2. @blend_enable 1 @color 1. 1. 1. 0.5"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-46",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 3,
									"outlettype" : [ "", "", "" ],
									"patching_rect" : [ 314.0, 73.0, 100.0, 22.0 ],
									"style" : "",
									"text" : "t l l l"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-43",
									"maxclass" : "message",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 639.0, 77.0, 29.5, 22.0 ],
									"style" : "",
									"text" : "$1"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-41",
									"maxclass" : "newobj",
									"numinlets" : 2,
									"numoutlets" : 2,
									"outlettype" : [ "", "" ],
									"patching_rect" : [ 639.0, 47.0, 61.0, 22.0 ],
									"style" : "",
									"text" : "route dim"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-40",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 639.0, 15.0, 75.0, 22.0 ],
									"style" : "",
									"text" : "jit.matrixinfo"
								}

							}
, 							{
								"box" : 								{
									"fontface" : 0,
									"fontname" : "Arial",
									"fontsize" : 12.0,
									"id" : "obj-39",
									"maxclass" : "jit.fpsgui",
									"mode" : 3,
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "", "" ],
									"patching_rect" : [ 177.5, 97.5, 80.0, 35.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-36",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "jit_matrix", "" ],
									"patching_rect" : [ 639.0, 286.0, 117.0, 22.0 ],
									"style" : "",
									"text" : "jit.pcl.segment.color"
								}

							}
, 							{
								"box" : 								{
									"fontface" : 0,
									"fontname" : "Arial",
									"fontsize" : 12.0,
									"id" : "obj-10",
									"maxclass" : "jit.fpsgui",
									"mode" : 3,
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "", "" ],
									"patching_rect" : [ 487.0, 353.0, 80.0, 35.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-33",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 48.0, 176.0, 100.0, 22.0 ],
									"style" : "",
									"text" : "loadmess 1"
								}

							}
, 							{
								"box" : 								{
									"attr" : "automatic",
									"id" : "obj-9",
									"maxclass" : "attrui",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 872.5, 421.0, 150.0, 22.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"attr" : "automatic",
									"id" : "obj-8",
									"maxclass" : "attrui",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 314.0, 414.0, 150.0, 22.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-6",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "jit_matrix", "" ],
									"patching_rect" : [ 1022.5, 353.0, 100.0, 22.0 ],
									"style" : "",
									"text" : "jit.pcl.convexhull"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-7",
									"linecount" : 7,
									"maxclass" : "newobj",
									"numinlets" : 9,
									"numoutlets" : 2,
									"outlettype" : [ "jit_matrix", "" ],
									"patching_rect" : [ 1022.5, 462.0, 124.0, 102.0 ],
									"style" : "",
									"text" : "jit.gl.mesh points @layer 0 @point_size 2. @blend_enable 1 @color 0. 1. 1. 0.5 @draw_mode line_loop"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-5",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "jit_matrix", "" ],
									"patching_rect" : [ 465.0, 326.0, 100.0, 22.0 ],
									"style" : "",
									"text" : "jit.pcl.convexhull"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-2",
									"linecount" : 7,
									"maxclass" : "newobj",
									"numinlets" : 9,
									"numoutlets" : 2,
									"outlettype" : [ "jit_matrix", "" ],
									"patching_rect" : [ 465.0, 462.0, 124.0, 102.0 ],
									"style" : "",
									"text" : "jit.gl.mesh points @layer 0 @point_size 2. @blend_enable 1 @color 1. 0. 1. 0.5 @draw_mode line_loop"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-1",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "", "" ],
									"patching_rect" : [ 130.0, 407.5, 107.0, 22.0 ],
									"style" : "",
									"text" : "jit.gl.handle points"
								}

							}
, 							{
								"box" : 								{
									"attr" : "point_size",
									"id" : "obj-11",
									"maxclass" : "attrui",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 864.0, 632.0, 150.0, 22.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-12",
									"maxclass" : "jit.pwindow",
									"name" : "u890000978",
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "", "" ],
									"patching_rect" : [ 1022.5, 215.0, 134.0, 21.0 ]
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-13",
									"maxclass" : "jit.pwindow",
									"name" : "u943000981",
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "", "" ],
									"patching_rect" : [ 876.5, 215.0, 134.0, 21.0 ]
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-14",
									"linecount" : 6,
									"maxclass" : "newobj",
									"numinlets" : 9,
									"numoutlets" : 2,
									"outlettype" : [ "jit_matrix", "" ],
									"patching_rect" : [ 885.0, 476.0, 125.0, 89.0 ],
									"style" : "",
									"text" : "jit.gl.mesh points @layer 0 @draw_mode points @point_size 2. @blend_enable 1 @color 1. 1. 1. 0.5"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-15",
									"linecount" : 3,
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 3,
									"outlettype" : [ "jit_matrix", "jit_matrix", "" ],
									"patching_rect" : [ 885.0, 124.0, 83.0, 49.0 ],
									"style" : "",
									"text" : "jit.unpack 2 @offset 0 3 @jump 3 3"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-17",
									"maxclass" : "button",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "bang" ],
									"patching_rect" : [ 830.0, 143.0, 24.0, 24.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"attr" : "draw_mode",
									"id" : "obj-18",
									"maxclass" : "attrui",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 864.0, 595.0, 150.0, 22.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"comment" : "",
									"id" : "obj-24",
									"maxclass" : "inlet",
									"numinlets" : 0,
									"numoutlets" : 1,
									"outlettype" : [ "jit_matrix" ],
									"patching_rect" : [ 885.0, 73.0, 30.0, 30.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"fontface" : 0,
									"fontname" : "Arial",
									"fontsize" : 12.0,
									"id" : "obj-3",
									"maxclass" : "jit.fpsgui",
									"mode" : 3,
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "", "" ],
									"patching_rect" : [ 210.0, 666.0, 80.0, 35.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-4",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "jit_matrix", "" ],
									"patching_rect" : [ 210.0, 638.0, 98.0, 22.0 ],
									"style" : "",
									"text" : "jit.pcl.convexhull"
								}

							}
, 							{
								"box" : 								{
									"attr" : "point_size",
									"id" : "obj-34",
									"maxclass" : "attrui",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 314.0, 614.0, 150.0, 22.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-28",
									"maxclass" : "jit.pwindow",
									"name" : "u264001079",
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "", "" ],
									"patching_rect" : [ 387.5, 256.0, 134.0, 21.0 ]
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-27",
									"maxclass" : "jit.pwindow",
									"name" : "u258001076",
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "", "" ],
									"patching_rect" : [ 177.5, 274.0, 134.0, 21.0 ]
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-26",
									"linecount" : 6,
									"maxclass" : "newobj",
									"numinlets" : 9,
									"numoutlets" : 2,
									"outlettype" : [ "jit_matrix", "" ],
									"patching_rect" : [ 314.0, 476.0, 125.0, 89.0 ],
									"style" : "",
									"text" : "jit.gl.mesh points @layer 0 @draw_mode points @point_size 2. @blend_enable 1 @color 1. 1. 1. 0.5"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-23",
									"maxclass" : "toggle",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "int" ],
									"parameter_enable" : 0,
									"patching_rect" : [ 48.0, 217.0, 24.0, 24.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-21",
									"maxclass" : "newobj",
									"numinlets" : 2,
									"numoutlets" : 1,
									"outlettype" : [ "bang" ],
									"patching_rect" : [ 48.0, 256.0, 65.0, 22.0 ],
									"style" : "",
									"text" : "qmetro 33"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-20",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "bang", "erase" ],
									"patching_rect" : [ 48.0, 414.0, 57.0, 22.0 ],
									"style" : "",
									"text" : "t b erase"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-19",
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "bang", "" ],
									"patching_rect" : [ 48.0, 588.0, 100.0, 22.0 ],
									"style" : "",
									"text" : "jit.window points"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-16",
									"linecount" : 3,
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 3,
									"outlettype" : [ "jit_matrix", "jit_matrix", "" ],
									"patching_rect" : [ 314.0, 196.0, 83.0, 49.0 ],
									"style" : "",
									"text" : "jit.unpack 2 @offset 0 3 @jump 3 3"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-22",
									"linecount" : 4,
									"maxclass" : "newobj",
									"numinlets" : 1,
									"numoutlets" : 2,
									"outlettype" : [ "bang", "" ],
									"patching_rect" : [ 48.0, 462.0, 143.0, 62.0 ],
									"style" : "",
									"text" : "jit.gl.render points @erase_color 0. 0. 0. 1. @blend_enable 1 @depth_enable 1"
								}

							}
, 							{
								"box" : 								{
									"id" : "obj-25",
									"maxclass" : "button",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "bang" ],
									"patching_rect" : [ 259.0, 143.0, 24.0, 24.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"attr" : "draw_mode",
									"id" : "obj-30",
									"maxclass" : "attrui",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 314.0, 577.0, 150.0, 22.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"comment" : "",
									"id" : "obj-31",
									"maxclass" : "inlet",
									"numinlets" : 0,
									"numoutlets" : 1,
									"outlettype" : [ "jit_matrix" ],
									"patching_rect" : [ 314.0, 22.0, 30.0, 30.0 ],
									"style" : ""
								}

							}
, 							{
								"box" : 								{
									"attr" : "floating",
									"id" : "obj-29",
									"maxclass" : "attrui",
									"numinlets" : 1,
									"numoutlets" : 1,
									"outlettype" : [ "" ],
									"patching_rect" : [ 48.0, 558.0, 150.0, 22.0 ],
									"style" : ""
								}

							}
 ],
						"lines" : [ 							{
								"patchline" : 								{
									"destination" : [ "obj-22", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-1", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-14", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-11", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-12", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-15", 1 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-13", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-15", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-14", 3 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-15", 1 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-14", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-15", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-6", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-15", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-26", 3 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-16", 1 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-26", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-16", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-27", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-16", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-28", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-16", 1 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-5", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-16", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-14", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-18", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-22", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-20", 1 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-22", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-20", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-20", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-21", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-21", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-23", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-15", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-24", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-17", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-24", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-19", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-29", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-26", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-30", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-25", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-31", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-39", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-31", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-46", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-31", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-23", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-33", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-26", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-34", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-52", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-36", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-3", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-4", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-41", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-40", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-43", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-41", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-65", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-43", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-16", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-46", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-40", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-46", 2 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-56", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-46", 1 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-10", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-5", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-2", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-5", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-51", 3 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-52", 1 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-51", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-52", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-51", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-53", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-2", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-55", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-36", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-56", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-61", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-56", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-57", 1 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-59", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-7", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-6", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-56", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-60", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-51", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-62", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-57", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-65", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-60", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-65", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-2", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-69", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-7", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-70", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-26", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-8", 0 ]
								}

							}
, 							{
								"patchline" : 								{
									"destination" : [ "obj-14", 0 ],
									"disabled" : 0,
									"hidden" : 0,
									"source" : [ "obj-9", 0 ]
								}

							}
 ]
					}
,
					"patching_rect" : [ 93.5, 478.5, 125.0, 22.0 ],
					"saved_object_attributes" : 					{
						"description" : "",
						"digest" : "",
						"globalpatchername" : "",
						"style" : "",
						"tags" : ""
					}
,
					"style" : "",
					"text" : "p point-cloud-viewer"
				}

			}
, 			{
				"box" : 				{
					"fontface" : 0,
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"id" : "obj-24",
					"maxclass" : "jit.fpsgui",
					"mode" : 3,
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"patching_rect" : [ 116.0, 389.0, 80.0, 35.0 ],
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-29",
					"linecount" : 2,
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 3,
					"outlettype" : [ "jit_matrix", "", "" ],
					"patching_rect" : [ 93.5, 341.0, 84.0, 35.0 ],
					"style" : "",
					"text" : "jit.net.recv @port 4001"
				}

			}
, 			{
				"box" : 				{
					"fontface" : 0,
					"fontsize" : 12.0,
					"id" : "obj-13",
					"maxclass" : "o.display",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 434.0, 92.0, 402.0, 34.0 ],
					"text" : "/notice : \"udp connected: 1 jitter connected: 1\"",
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
					"patching_rect" : [ 352.0, 58.0, 101.0, 22.0 ],
					"style" : "",
					"text" : "o.route /api /data"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-1",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 352.0, 14.0, 135.0, 22.0 ],
					"style" : "",
					"text" : "udpreceive 4000 cnmat"
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
					"destination" : [ "obj-16", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-10", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-33", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-10", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-7", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-18", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-14", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-24", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-28", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-27", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-27", 1 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-28", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-30", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-28", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-24", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-29", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-32", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-29", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-36", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-29", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-22", 1 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-30", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-31", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-33", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-34", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-33", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-32", 1 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-37", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-38", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-37", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-42", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-37", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-12", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-38", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-27", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-40", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-48", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-47", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-56", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-48", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-48", 1 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-49", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-51", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-49", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-48", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-51", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-55", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-52", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-52", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-53", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-55", 1 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-53", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-55", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-54", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-57", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-55", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-33", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-56", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-33", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-57", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-10", 1 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-6", 0 ]
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
					"destination" : [ "obj-3", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-6", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-9", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-6", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-48", 2 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-60", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-51", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-60", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-52", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-61", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-55", 2 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-61", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-10", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-7", 0 ]
				}

			}
 ],
		"dependency_cache" : [ 			{
				"name" : "o.route.mxo",
				"type" : "iLaX"
			}
, 			{
				"name" : "o.display.mxo",
				"type" : "iLaX"
			}
, 			{
				"name" : "jit.pcl.convexhull.mxo",
				"type" : "iLaX"
			}
, 			{
				"name" : "jit.pcl.segment.color.mxo",
				"type" : "iLaX"
			}
, 			{
				"name" : "o.compose.mxo",
				"type" : "iLaX"
			}
, 			{
				"name" : "o.union.mxo",
				"type" : "iLaX"
			}
, 			{
				"name" : "o.select.mxo",
				"type" : "iLaX"
			}
, 			{
				"name" : "o.pack.mxo",
				"type" : "iLaX"
			}
, 			{
				"name" : "o.prepend.mxo",
				"type" : "iLaX"
			}
, 			{
				"name" : "mxj.mxo",
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
