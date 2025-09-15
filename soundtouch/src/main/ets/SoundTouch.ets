import { processAsync, processSync } from "libsoundtouch.so";
import { SoundTouchOptions } from "./SoundTouchOptions";
import { BusinessError } from "@kit.BasicServicesKit";

export class SoundTouch {
  private constructor() {
    // ...
  }

  public static process(input: string, output: string, options?: SoundTouchOptions): Promise<void> {
		return new Promise((resolve, reject) => {
      processAsync(input, output, options).then(() => {
        resolve();
      }).catch((error: BusinessError) => {
        reject(error);
      });
    });
	}

  public static processSync(input: string, output: string, options?: SoundTouchOptions) {
    processSync(input, output, options);
  }
}