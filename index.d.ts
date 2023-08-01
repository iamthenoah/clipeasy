export interface ClipboardOptions {
	/**
	 * Specify an interval (in ms) for how frequently a changes from the clipboard are watched.
	 *
	 * @default 500 .5 seconds
	 */
	interval?: number
}

export class Clipboard {
	constructor(options?: ClipboardOptions)
	/**
	 * Read the data in the clipboard.
	 *
	 * Note: if files have been copied, an empty string is returned.
	 *
	 * @returns clipboard text
	 */
	read(): string
	/**
	 * Writes text to the clipboard.
	 *
	 * @param text data to write
	 */
	write(text: string): void
	/**
	 * Read the file paths in the clipboard.
	 *
	 * Note: if no files have been copied, and empty array is returned.
	 *
	 * @returns file paths
	 */
	readFiles(): string[]
	/**
	 * Write file paths to the clipboard.
	 *
	 * @param paths file paths
	 */
	writeFiles(paths: string[]): void
	/**
	 * Clear the clipboard.
	 */
	clear(): void
	/**
	 * Stop the watcher from listening to clipboard changes.
	 */
	stop(): void
	/**
	 * Listen for text changes on the clipboard.
	 *
	 * @param event clipboard text change event
	 * @param listener callback
	 */
	on(event: 'text-changed', listener: (text: string) => void): this
	/**
	 * Listen for file changes on the clipboard.
	 *
	 * @param event clipboard file change event
	 * @param listener callback
	 */
	on(event: 'file-changed', listener: (paths: string[]) => void): this
	/**
	 * Listen for text changes on the clipboard.
	 *
	 * @param event clipboard text change event
	 * @param listener callback
	 */
	once(event: 'text-changed', listener: (text: string) => void): this
	/**
	 * Listen for file changes on the clipboard only once.
	 *
	 * @param event clipboard file change event
	 * @param listener callback
	 */
	once(event: 'file-changed', listener: (paths: string[]) => void): this
	/**
	 * Add a listener for text changes on the clipboard.
	 *
	 * @param event clipboard text change event
	 * @param listener callback
	 */
	addListener(event: 'text-changed', listener: (text: string) => void): this
	/**
	 * Add a listener for file changes on the clipboard.
	 *
	 * @param event clipboard file change event
	 * @param listener callback
	 */
	addListener(event: 'file-changed', listener: (paths: string[]) => void): this
	/**
	 * Remove a listener for text changes on the clipboard.
	 *
	 * @param event clipboard text change event
	 * @param listener callback
	 */
	removeListener(event: 'text-changed', listener: (text: string) => void): this
	/**
	 * Remove a listener for file changes on the clipboard.
	 *
	 * @param event clipboard file change event
	 * @param listener callback
	 */
	removeListener(event: 'file-changed', listener: (paths: string[]) => void): this
	/**
	 * Remove all listeners from a clipboard change event.
	 *
	 * @param event clipboard change event
	 */
	removeAllListeners(event?: 'text-changed' | 'file-changed'): this
	/**
	 * Get all listeners from a clipboard change event.
	 *
	 * @param event clipboard change event
	 * @returns all event listeners
	 */
	listeners(event: 'text-changed' | 'file-changed'): ((text: string) => void)[] | ((paths: string[]) => void)[]
	/**
	 * Emit an event for a clipboard text change.
	 *
	 * Note: avoid using as this may not reflect the clipboard.
	 *
	 * @param event clipboard text change event
	 * @param text data
	 */
	emit(event: 'text-changed', text: string): boolean
	/**
	 * Emit an event for a clipboard file change.
	 *
	 * Note: avoid using as this may not reflect the clipboard.
	 *
	 * @param event clipboard file change event
	 * @param paths file paths
	 */
	emit(event: 'file-changed', paths: string[]): boolean
}
