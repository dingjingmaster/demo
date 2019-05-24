import * as ActionType from './ActionTypes';
import AppDispatcher from './AppDispatcher';

export const increment = (counterCaption) => {
  AppDispatcher.dispatch({
      type: ActionType.INCREMENT,
      counterCaption: counterCaption
  });
};
export const decrement = (counterCaption) => {
    AppDispatcher.dispatch({
        type: ActionType.DECREMENT,
        counterCaption: counterCaption
    });
};