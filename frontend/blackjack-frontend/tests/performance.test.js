
// tests/performance.test.js
import { post } from "../src/api.js";

describe("Performance Test - 1000 Game Actions", () => {
  const username = "speedtester";
  const password = "fastpass";
  let session = "";

  beforeAll(async () => {
    await post("/register", `${username}:${password}`);
    session = await post("/login", `${username}:${password}`);
  });

  test("Run 1000 deal/hit/stand/status within 1 second", async () => {
    const start = Date.now();
    for (let i = 0; i < 250; i++) {
      await post("/deal", "", session);
      await post("/hit", "", session);
      await post("/stand", "", session);
      await post("/status", "", session);
    }
    const end = Date.now();
    const duration = end - start;
    console.log("Total ms:", duration);
    expect(duration).toBeLessThan(1000);
  });
});
