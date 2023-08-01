const { Clipboard } = require('./')

const clipboard = new Clipboard({
	interval: 100
})

clipboard.on('text-changed', console.log)
