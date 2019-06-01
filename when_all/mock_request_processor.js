'use strict';

const when = require('when');

/**
    @return {Promise.<number>}
*/
module.exports = function (requestFactory, processResponseItem) {
    const requests = [];
    let ok = true;

    // выполняем запросы, пока они не вернут false

    while (ok) {
        ok = requestFactory.next();
        if (ok) {
            requests.push(ok);
        }
    }
    
    // когда они выполнятся, посчитаем сумму
    return when.all(requests)
    .then((responses) => {
        let sum = 0;
        for (let r = 0; r < responses.length; r++) {
            for (let i = 0; i < responses[r].length; i++) {
                sum += processResponseItem(responses[r][i].a);
            }
        }
        return sum;
    })
};