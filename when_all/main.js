'use strict';

const when = require('when');
const assert = require('assert');

const N = 1000000;
const M = 10;
let correctSum = 0;

const processResponseItem = function (item) {
    return Math.sqrt(item);
};

/**
    @return {Array.<{a: number}>}
*/
const dbResponse = function () {
    const ar = [];
    for (let i = 0; i < N; i++) {
        ar.push({a: parseInt(Math.random()*M, 10)})
        correctSum += processResponseItem(ar[ar.length - 1].a);
    }
    return ar;
}

// параметры задачи
const delays = [10, 700, 100, 300, 1000, 200, 50, 1000, 200, 50, 10, 700, 100, 300, 1000, 200, 50, 1000, 200, 50, 2000];

const responses = delays.map(() => {
    return dbResponse();
})

const RequestFactory = function () {
    this.n = -1;
    /**
        ф-я выполняющая запрос, возвращает Promise или false, когда задачи кончились
        @return {Promise|false}
    */
    this.next = function () {
        this.n++;
        if (!delays[this.n]) {
            return false;
        }
        
        // Эмуляция не мгновенного ответа
        return when.resolve(responses[this.n]).delay(delays[this.n]);
    };
};

const processors = {
    /**
        @return {Promise.<number>}
    */
    mockRequestProcessor: require('./mock_request_processor'),
    /**
        @return {Promise.<number>}
    */
    requestProcessor: process.argv[2] == null ? null : require('./' + process.argv[2])
};
const names = Object.keys(processors);
const times = {};
let start;

when.iterate(
    (i) => i + 1,
    (i) => i == names.length,
    (i) => {
        if (processors[names[i]]) {
            start = +new Date();
            return processors[names[i]](new RequestFactory(), processResponseItem)
                .then((res) => {
                    times[names[i]] = +new Date() - start;
                    console.log(names[i], 'took', times[names[i]], 'msec');
                    assert.equal(Math.floor(res), Math.floor(correctSum));
                })
                .catch((e) => {
                    console.error(names[i], e);
                });
        }
    },
    0
)
.then(() => {
    if (times.mockRequestProcessor && times.requestProcessor) {
        assert(times.mockRequestProcessor > times.requestProcessor * 1.1, 'implemented processor should be more than 10% faster than mock processor');
    } else {
        console.log('to check your request processor run: node main.js <filename>');
    }
})
.catch((e) => {
    console.log(e);
});
