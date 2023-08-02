const binding = require('node-gyp-build')(__dirname)
const EventEmitter = require('events')

class Clipboard extends EventEmitter {
	constructor(options) {
		super(options)

		let previousText = binding.read()
		let previousFiles = binding.readFiles()

		this.watcherId = setInterval(() => {
			if (binding.read() !== previousText) {
				previousText = binding.read()
				this.emit('text-changed', previousText)
			} else if (!binding.readFiles().every((v, i) => v === previousFiles[i])) {
				previousFiles = binding.readFiles()
				this.emit('file-changed', previousFiles)
			}
		}, options?.interval ?? 500)
	}

	stop() {
		this.watcherId && clearInterval(this.watcherId)
	}
}

/**
 * Append all native functions of `build` to `Clipboard`:
 *
 * @method read(): string
 * @method write(string): void
 * @method readFiles(): string[]
 * @method writeFiles(string[]): void
 * @method clear(): void
 */
Object.assign(Clipboard.prototype, binding)

module.exports = { Clipboard }
