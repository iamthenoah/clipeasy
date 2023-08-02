{
	"targets": [
		{
      "target_name": "bindings",
      "sources": [
        "src/export.cc"
      ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")"
      ],
      "dependencies": [
        "<!(node -p \"require('node-addon-api').gyp\")"
      ],
			"cflags!": [
				"-fno-exceptions"
			],
			"cflags_cc!": [
				"-fno-exceptions"
			],
			"conditions": [
				[
					'OS=="mac"',
					{
						"sources": [
							"src/clipboard_darwin.mm"
						],
						"link_settings": {
							"libraries": [
								"-framework Cocoa",
								"-framework CoreFoundation"
							]
						},
						"xcode_settings": {
							"GCC_ENABLE_CPP_EXCEPTIONS": "YES",
							"CLANG_ENABLE_OBJC_ARC": "YES",
							"OTHER_CFLAGS": [
								"-ObjC++",
								"-std=c++17"
							]
						}
					}
				],
        [
          'OS=="win"',
          {
            "sources": [
              "src/clipboard_win32.cc"
            ],
            "msvs_settings": {
              "VCCLCompilerTool": {
                "AdditionalOptions": [
                  "/std:c++17"
                ]
              }
            }
          }
        ],
        [
          'OS=="linux"',
          {
            "sources": [
              "src/clipboard_linux.cc"
            ]
          }
        ]
			]
		}
	]
}
