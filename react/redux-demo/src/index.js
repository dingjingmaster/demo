import React from 'react';
import ReactDOM from 'react-dom';
import ControlPanel from './views/ControlPanel';
import * as serviceWorker from './serviceWorker';

ReactDOM.render(
    <ControlPanel />,
    document.getElementById('root')
);
serviceWorker.unregister();
