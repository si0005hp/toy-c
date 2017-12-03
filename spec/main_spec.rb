describe 'main' do
  before do
  end

  def run_script(commands)
    raw_output = nil
    IO.popen("./main", "r+") do |pipe|
      commands.each do |command|
        pipe.puts command
      end

      pipe.close_write

      # Read entire output
      raw_output = pipe.gets(nil)
    end
    raw_output.split("\n")
  end

  it 'integer' do
    result = run_script([
      "1",
      "999",
    ])
    expect(result).to eq([
      "1",
      "999",
    ])
  end
  
  it 'add' do
    result = run_script([
      "1+2",
      "1 + 2",
      "1 + 2 + 3 + 4 + 5",
    ])
    expect(result).to eq([
      "3",
      "3",
      "15",
    ])
  end
  
  it 'sub' do
    result = run_script([
      "3-1",
      "10 - 3 - 2 - 1",
      "5 + 3 - 2 + 1 - 4 + 9",
    ])
    expect(result).to eq([
      "2",
      "4",
      "12",
    ])
  end
  
  it 'mul' do
    result = run_script([
      "2*5",
      "2 * 3 * 4",
      "1 * 2 + 3 + 4 * 5",
      "1 * 2 + 3 - 4 * 5",
    ])
    expect(result).to eq([
      "10",
      "24",
      "25",
      "-15",
    ])
  end
  
  it 'mul' do
    result = run_script([
      "10/2",
      "99 / 3 / 11",
      "10 / 2 * 3 + 5 - 3 + 4 * 2 / 8",
    ])
    expect(result).to eq([
      "5",
      "3",
      "18",
    ])
  end
  
  it 'paren' do
    result = run_script([
      "(1)",
      "(1 + 3)",
      "(2 * 3)",
      "(1 + 3) * 5",
    ])
    expect(result).to eq([
      "1",
      "4",
      "6",
      "20",
    ])
  end
end
