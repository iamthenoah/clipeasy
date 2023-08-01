const EventEmitter = require('events')
const build = require('./build')

class Clipboard extends EventEmitter {
	constructor(options) {
		super(options)

		let previousText = build.read()
		let previousFiles = build.readFiles()

		this.eventId = setInterval(() => {
			if (build.read() !== previousText) {
				previousText = build.read()
				this.emit('text-changed', previousText)
			} else if (!build.readFiles().every((v, i) => v === previousFiles[i])) {
				previousFiles = build.readFiles()
				this.emit('file-changed', previousFiles)
			}
		}, options?.interval ?? 500)
	}

	stop() {
		this.eventId && clearInterval(this.eventId)
	}
}

/**
 * Append all native functions of `build` to `Clipboard`:
 *
 * @method read()
 * @method write(string)
 * @method readFiles()
 * @method writeFiles(string[])
 * @method clear()
 */
Object.assign(Clipboard.prototype, build)

module.exports = Clipboard
