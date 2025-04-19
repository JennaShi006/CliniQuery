import './App.css';
import Header from './Header';
import { HomePage } from './Pages/HomePage';

function App() {
  const handleNameChange = () => {
    const names = ['bob', 'dave', 'jenna'];
    const int = Math.floor(Math.random() *3);
    return names[int];
  }


  return (
    <div className="App">
      <Header/>
      <HomePage />
    </div>
  );
}

export default App;
