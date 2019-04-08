<?php
namespace app\controllers;
use yii\web\Controller;

class IndexController extends Controller {
    public $layout = false;

    public function actionIndex() {
        return $this->render( '@app/views/index', [
            'title'                 => '测试',

        ]);
    }
}
