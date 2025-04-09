import { useEffect, useState } from 'react';
import { post } from '../api';

export default function GameStatus({ session }) {
  const [status, setStatus] = useState('');

  const fetchStatus = async () => {
    const data = await post('/status', '', session);
    setStatus(data);
  };

  useEffect(() => {
    fetchStatus();
    const interval = setInterval(fetchStatus, 2000);
    return () => clearInterval(interval);
  }, [session]);

  return (
    <div>
      <h3>📝 Game Status</h3>
      <pre>{status}</pre>
    </div>
  );
}