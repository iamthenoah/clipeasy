const prebuildify = require('util').promisify(require('prebuildify'))

const build = async () => {
	let { platform } = process
	let archs = []

	if (platform === 'darwin') {
		archs = ['x64', 'arm64']
	} else if (platform === 'win32') {
		archs = ['x64', 'ia32']
	} else if (platform === 'linux') {
		archs = ['x64', 'arm64']
	}

	for (const arch of archs) {
		await prebuildify({ platform, arch, napi: true })
	}
}

build()
