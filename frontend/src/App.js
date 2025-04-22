import './App.css';
import { HomePage } from './Pages/HomePage';


function App() {

  return (
    <div className="App">
      <div style={{ display: 'flex', 
        justifyContent: 'center', 
        alignItems: 'center', 
        gap: '1rem' }}>
          
        <img src="/CliniQueryCat.png" alt="CliniQuery Logo" 
        style={{ position: 'absolute', 
          left: '25px', 
          top: '25px', 
          width: '10vmin', 
          height: '10vmin' }} />

        <h1 style={{ fontSize: '1.5rem', fontWeight: 'bold' }}>CliniQuery</h1>
      </div>
      <h4>Created By: Jenna Shi, Derrick Ma, and Tanvi Garg</h4>
      <HomePage />
    </div>
  );
}

export default App;
