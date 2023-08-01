const EventEmitter = require('events')
const build = require('./build')

module.exports = class Clipboard extends EventEmitter {
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

	read() {
		return build.read()
	}

	write(data) {
		build.write(data)
	}

	readFiles() {
		return build.readFiles()
	}

	writeFiles(paths) {
		build.writeFiles(paths)
	}

	clear() {
		build.clear()
	}

	stop() {
		this.eventId && clearInterval(this.eventId)
	}
}
