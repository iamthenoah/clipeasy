const path = require('path').resolve(__dirname, '../')
const build = require('node-gyp-build')(path)

module.exports = build
