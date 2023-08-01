export interface ClipboardOptions {
	interval?: number
}

export class Clipboard {
	constructor(options?: ClipboardOptions)

	read(): string
	write(text: string): void
	readFiles(): string[]
	writeFiles(paths: string[]): void
	clear(): void
	stop(): void

	on(event: 'text-changed', listener: (text: string) => void): this
	on(event: 'file-changed', listener: (paths: string[]) => void): this

	once(event: 'text-changed', listener: (text: string) => void): this
	once(event: 'file-changed', listener: (paths: string[]) => void): this

	addListener(event: 'text-changed', listener: (text: string) => void): this
	addListener(event: 'file-changed', listener: (paths: string[]) => void): this

	removeListener(event: 'text-changed', listener: (text: string) => void): this
	removeListener(event: 'file-changed', listener: (paths: string[]) => void): this
	removeAllListeners(event?: 'text-changed' | 'file-changed'): this

	listeners(event: 'text-changed' | 'file-changed'): ((text: string) => void)[]

	emit(event: 'text-changed', text: string): boolean
	emit(event: 'file-changed', paths: string[]): boolean
}
