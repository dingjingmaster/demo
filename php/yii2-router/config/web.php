<?php

$config = [
    'id' => 'demo',
    'basePath' => dirname(__DIR__),
    'language' => 'zh-cn',
    'aliases' => [
        '@bower' => '@vendor/bower-asset',
        '@npm'   => '@vendor/npm-asset',
    ],
    'components' => [
        'errorHandler' => [
            'errorAction' => 'site/error',
        ],
        'log' => [
            'traceLevel' => YII_DEBUG ? 3 : 0,
            'targets' => [
                [
                    'class' => 'yii\log\FileTarget',
                    'levels' => ['error', 'warning'],
                ],
            ],
        ],

        /* url 管理器 */
        'urlManager' => [
            'enablePrettyUrl' => true,              // 美化 url
            'showScriptName' => true,               // 隐藏 index.php
            'suffix' => '.html',                    // 后缀
            'rules' => [
                'GET posts/<year:(2012|2013)>/'                                 => 'index/index',
            ],
        ],
    ],
];
return $config;
