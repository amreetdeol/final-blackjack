// tests/functional.test.js
import { post } from "../src/api.js";

const users = [
  { username: "player1", password: "pass1" },
  { username: "player2", password: "pass2" },
  { username: "player3", password: "pass3" },
  { username: "player4", password: "pass4" },
];

describe("Functional Test - Multiplayer Game Flow", () => {
  const sessions = {};

  test("Register and login all users", async () => {
    for (const user of users) {
      await post("/register", `${user.username}:${user.password}`);
      const token = await post("/login", `${user.username}:${user.password}`);
      expect(token.startsWith("session-")).toBe(true);
      sessions[user.username] = token;
    }
  });

  test("Each user performs deal", async () => {
    for (const user of users) {
      const res = await post("/deal", "", sessions[user.username]);
      expect(res.toLowerCase()).toContain("hand");
    }
  });
});

