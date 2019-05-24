import React, { Component } from 'react';
import PropTypes from 'prop-types'
import * as Actions from '../Actions.js';
import CounterStore from '../CounterStore.js';

const buttonStyle = {
    margin: '10px'
};

class Counter extends Component {
    constructor(props) {
        super(props);
        this.onChange = this.onChange.bind(this);
        this.onClickIncrementButton = this.onClickIncrementButton.bind(this);
        this.onClickDecrementButton = this.onClickDecrementButton.bind(this);
        this.state = {
            count: CounterStore.getCounterValues()[props.caption]
        }
    }
    shouldComponentUpdate(nextProps, nextState) {
        return (nextProps.caption !== this.props.caption) ||
            (nextState.count !== this.state.count);
    }
    componentDidMount() {
        CounterStore.addChangeListener(this.onChange);
    }
    componentWillUnmount() {
        CounterStore.removeChangeListener(this.onChange);
    }
    onChange() {
        const newCount = CounterStore.getCounterValues()[this.props.caption];
        this.setState({count: newCount});
    }
    onClickIncrementButton() {
        Actions.increment(this.props.caption);
    }
    onClickDecrementButton() {
        Actions.decrement(this.props.caption);
    }
    render() {
        const {caption} = this.props;
        return (
            <div>
                <button style={buttonStyle} onClick={this.onClickIncrementButton}>+</button>
                <button style={buttonStyle} onClick={this.onClickDecrementButton}>-</button>
                <span>{caption} count: {this.state.count}</span>
            </div>
        );
    }
}
Counter.propTypes = {
    caption: PropTypes.string.isRequired
};
export default Counter;