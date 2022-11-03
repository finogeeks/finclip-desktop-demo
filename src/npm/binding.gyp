{
    "targets": [
        {
            "target_name": "finclip",
            "win_delay_load_hook": "true",
            "sources": [
                "finclip.cpp"
            ],
            "cflags_cc": [
                "-std=c++17"
            ],
            'include_dirs': ["<!@(node -p \"require('node-addon-api').include\")"],
            'dependencies': ["<!(node -p \"require('node-addon-api').gyp\")"],
            "libraries": [
                "../FinClipSDKWrapper.lib",
            ],
            "defines": ["_UNICODE", "UNICODE", "NAPI_DISABLE_CPP_EXCEPTIONS"],
            "copies": [
                {
                    "destination": "<(module_root_dir)/build/Release/",
                    "files": [
                        "./FinClipSDKWrapper.lib",
                        "./FinClipSDKWrapper.dll",
                        "../../vendor/win/x64/finclip.exe",
                        "../../vendor/win/x64/finclip.exp",
                        "../../vendor/win/x64/finclip.lib",
                        "package.json"
                    ]
                },
            ],
            "conditions": [
                ['OS=="mac"', {
                    "xcode_settings": {
                        'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
                        "CLANG_CXX_LIBRARY": "libc++",
                        "CLANG_CXX_LANGUAGE_STANDARD": "c++17",
                        'MACOSX_DEPLOYMENT_TARGET': '10.14'
                    }
                }],
                ['OS=="win"', {
                    "msvs_settings": {
                        "VCCLCompilerTool": {
                            "AdditionalOptions": ["-std:c++17", ],
                        },
                    },
                }]
            ]
        }
    ],
    "msbuild_settings": {
        "ClCompile": {
            "LanguageStandard": "stdcpp17"
        }
    }
}
