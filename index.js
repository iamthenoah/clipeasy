const binding = require('node-gyp-build')(__dirname)
const EventEmitter = require('events')

class Clipboard extends EventEmitter {
	constructor(options) {
		super(options)

		let previous = binding.readFiles().length ? binding.readFiles() : binding.read()

		this.watcherId = setInterval(() => {
			const text = binding.read()
			const files = binding.readFiles()

			// funky logic. this is because binding.read() also returns file names that have been copied
			if (files.length) {
				if (!Array.isArray(previous) || files.some((v, i) => v !== previous[i])) {
					this.emit('file-changed', files)
					previous = files
				}
			} else if (text !== previous) {
				this.emit('text-changed', text)
				previous = text
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
