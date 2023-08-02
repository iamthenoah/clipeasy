const path = require('path').resolve(__dirname, '../')
const binding = require('node-gyp-build')(path)

module.exports = binding
