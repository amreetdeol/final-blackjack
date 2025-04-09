// tests/stress.test.js
import { post } from "../src/api.js";

const players = [
  { username: "stress1", password: "stress" },
  { username: "stress2", password: "stress" },
  { username: "stress3", password: "stress" },
  { username: "stress4", password: "stress" },
];

describe("Stress Test - Concurrent Players", () => {
  const sessions = {};

  beforeAll(async () => {
    for (const p of players) {
      await post("/register", `${p.username}:${p.password}`);
      const token = await post("/login", `${p.username}:${p.password}`);
      sessions[p.username] = token;
    }
  });

  test("All players deal/hit in parallel", async () => {
    const promises = players.map(p => post("/deal", "", sessions[p.username]));
    const results = await Promise.all(promises);
    results.forEach(r => expect(r.toLowerCase()).toContain("hand"));
  });

  test("All players hit in parallel", async () => {
    const promises = players.map(p => post("/hit", "", sessions[p.username]));
    const results = await Promise.all(promises);
    results.forEach(r => expect(typeof r).toBe("string"));
  });
});
