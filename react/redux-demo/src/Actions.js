import * as ActionTypes from './ActionTypes';
export const increment = (counterCaption) => {
    return {
        type: ActionTypes.INSCREMENT,
        counterCaption: counterCaption
    };
};
export const decrement = (counterCaption) => {
    return {
        type: ActionTypes.DECREMENT,
        counterCaption: counterCaption
    };
};