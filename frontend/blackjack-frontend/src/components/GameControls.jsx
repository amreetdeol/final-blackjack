import React, { useState } from "react";
import { post } from "../api";

export default function GameControls({ username, session }) {
  const [output, setOutput] = useState("");

  const call = async (action) => {
    const res = await post(`/${action}`, "", session);
    setOutput(res);
  };

  return (
    <div style={{ border: "1px solid gray", padding: "1rem", width: "300px" }}>
      <h3>{username}</h3>
      <button onClick={() => call("deal")}>Deal</button>
      <button onClick={() => call("hit")}>Hit</button>
      <button onClick={() => call("stand")}>Stand</button>
      <button onClick={() => call("status")}>Status</button>
      <pre style={{ marginTop: "1rem", whiteSpace: "pre-wrap" }}>{output}</pre>
    </div>
  );
}