import { post } from "../src/api.js"; 

describe("UserManager - Registration & Login", () => {
  const baseUser = { username: "testuser1", password: "testpass" };

  test("Register new user", async () => {
    const res = await post("/register", `${baseUser.username}:${baseUser.password}`);
    expect(res.includes("User registered") || res.includes("Username already exists")).toBeTruthy();
  });

  test("Login with correct credentials", async () => {
    const res = await post("/login", `${baseUser.username}:${baseUser.password}`);
    expect(res.startsWith("session-")).toBe(true);
  });

  test("Login with wrong password", async () => {
    const res = await post("/login", `${baseUser.username}:wrongpass`);
    expect(res).toBe("Login failed");
  });
});
